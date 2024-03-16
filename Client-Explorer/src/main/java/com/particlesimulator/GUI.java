package com.particlesimulator;

import imgui.*;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImBoolean;
import imgui.type.ImInt;

public class GUI {
    private static final int menuWindowWidth = 200;
    private static final int menuWindowHeight = 100;
    private static final int menuWindowPosX = 0;
    private static final int menuWindowPosY = Utils.windowHeight - menuWindowHeight;
    private static final int menuWindowFlags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse;

    private static final int playerWindowWidth = 200;
    private static final int playerWindowHeight = 100;
    private static final int playerWindowPosX = (Utils.windowWidth/2) - (playerWindowWidth/2);
    private static final int playerWindowPosY = (Utils.windowHeight/2) - (playerWindowHeight/2);
    private static final int playerWindowFlags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse;

    private ImInt fps = new ImInt(60);
    private ImInt posX = new ImInt(0);
    private ImInt posY = new ImInt(0);
    private ImBoolean spawned = new ImBoolean(false);

    public GUI() {
        init();
    }

    private void init() {
        ImGui.createContext();
        ImGuiIO io = ImGui.getIO();
        
        // Set dark mode
        ImGui.styleColorsDark();

        ImGui.newFrame();
    }

    public void update() {
        menuWindow();

        if (!spawned.get()) { spawnPlayerWindow(); } 
        else { }
    }

    public void menuWindow() {
        ImGui.begin("Menu", new ImBoolean(true), menuWindowFlags);
        ImGui.setWindowPos(menuWindowPosX, menuWindowPosY);
        ImGui.setWindowSize(menuWindowWidth, menuWindowHeight);
        
        ImGui.text("Particle Simulator Client");
        ImGui.text("FPS: " + fps.get());
        ImGui.text("Total time: " + ImGui.getTime());

        ImGui.end();
    }

    public void spawnPlayerWindow() {
        ImGui.begin("Player Spawn", spawned, playerWindowFlags);
        ImGui.setWindowPos(playerWindowPosX, playerWindowPosY);
        ImGui.setWindowSize(playerWindowWidth, playerWindowHeight);

        ImGui.text("Spawn Coordinates");
        ImGui.inputInt("X", posX);
        ImGui.inputInt("Y", posY);
        clampWindowPos();

        if (ImGui.button("Spawn")) {
            spawned.set(true);
        }

        ImGui.end();
    }

    public void render() { ImGui.render(); }
    public void shutdown() { ImGui.destroyContext(); }

    /**
     * Clamp the player position to the window bounds to
     * avoid player going off-screen upon spawning.
     */
    private void clampWindowPos() {
        int x = posX.get();
        int y = posY.get();

        if (x < 0) { posX.set(0); }
        else if (x > Utils.windowWidth) { posX.set(Utils.windowWidth); }

        if (y < 0) { posY.set(0); }
        else if (y > Utils.windowHeight) { posY.set(Utils.windowHeight); }
    }
}
