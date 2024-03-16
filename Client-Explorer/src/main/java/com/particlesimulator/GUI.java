package com.particlesimulator;

import imgui.*;
import imgui.type.ImBoolean;
import imgui.type.ImInt;

public class GUI {
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
        if (!spawned.get()) {
            spawnSpriteWindow();
        } else {

        }
    }

    public void spawnSpriteWindow() {
        ImGui.begin("Sprite");
        ImGui.text("Sprite Window");
        
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
