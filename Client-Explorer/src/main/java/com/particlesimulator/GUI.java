package com.particlesimulator;

import imgui.*;
import imgui.flag.ImGuiWindowFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;
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

    // Implementations
    private ImGuiImplGlfw imGLFW;
    private ImGuiImplGl3 imGL3;

    // Mounts glfw window to imgui
    public GUI(long glfwWindow) {
        init(glfwWindow);
    }

    private void init(long glfwWindow) {
        ImGui.createContext();
        ImGuiIO io = ImGui.getIO();
        io.setDisplaySize(Utils.windowWidth, Utils.windowHeight);
        io.setIniFilename(null); // Disable .ini file
    
        // Fonts
        io.getFonts().addFontDefault();
        io.getFonts().build();

        // Setup platform/renderer bindings
        imGLFW = new ImGuiImplGlfw();
        imGLFW.init(glfwWindow, false);

        imGL3 = new ImGuiImplGl3();
        imGL3.init("#version 330");
        
        // Set dark mode
        ImGui.styleColorsDark();
    }

    public void newFrame() { 
        //  GLFW Frame
        imGLFW.newFrame();
        ImGui.newFrame();
    }

    public void update() {
        ImGui.begin("Test");
        ImGui.text("Hello, world!");
        ImGui.end();
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

    public void render() { 
        ImGui.render(); 
        imGL3.renderDrawData(ImGui.getDrawData());
    }
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
