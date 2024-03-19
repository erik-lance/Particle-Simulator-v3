package com.particlesimulator;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.objects.ObjectManager;

import imgui.*;
import imgui.flag.ImGuiConfigFlags;
import imgui.flag.ImGuiWindowFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;
import imgui.type.ImBoolean;
import imgui.type.ImInt;

import org.lwjgl.glfw.GLFWKeyCallback;

public class GUI {
    private static int menuWindowWidth = 200;
    private static final int menuWindowHeight = 100;
    private static final int menuWindowPosX = 0;
    private static final int menuWindowPosY = Utils.windowHeight - menuWindowHeight;
    private static final int menuWindowPosYClosed = Utils.windowHeight;
    private static final int menuWindowFlags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove;

    private static final int playerWindowWidth = 200;
    private static final int playerWindowHeight = 200;
    private static final int playerWindowPosX = (Utils.windowWidth/2) - (playerWindowWidth/2);
    private static final int playerWindowPosY = (Utils.windowHeight/2) - (playerWindowHeight/2);
    private static final int playerWindowFlags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoCollapse;

    // ImGui variables
    private float fps =  0.0f;
    private ImInt posX = new ImInt(0);
    private ImInt posY = new ImInt(0);
    private ImBoolean spawned = new ImBoolean(false);

    // Implementations
    private ImGuiImplGlfw imGLFW;
    private ImGuiImplGl3 imGL3;
    private GLFWKeyCallback keyCallback;

    // Objects
    private ObjectManager objectManager;

    // Mounts glfw window to imgui
    public GUI(long glfwWindow, ObjectManager objectManager) {
        init(glfwWindow);
        this.objectManager = objectManager;
    }

    private void init(long glfwWindow) {
        ImGui.createContext();
        ImGuiIO io = ImGui.getIO();
        io.setDisplaySize(Utils.windowWidth, Utils.windowHeight);
        io.setIniFilename(null); // Disable .ini file

        // Set key mappings
        io.setConfigFlags(ImGuiConfigFlags.NavEnableKeyboard); // Enable keyboard controls

        // Fonts
        io.getFonts().addFontDefault();
        io.getFonts().build();

        // Setup platform/renderer bindings
        imGLFW = new ImGuiImplGlfw();
        imGLFW.init(glfwWindow, true);

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
        menuWindow();

        if (!spawned.get()) { spawnPlayerWindow(); } 
        else { }
    }

    public void menuWindow() {
        ImGui.begin("Menu", new ImBoolean(true), menuWindowFlags);
        
        // Set window position based on whether the window is open or closed
        if (ImGui.isWindowCollapsed()) { ImGui.setWindowPos(menuWindowPosX, menuWindowPosYClosed); }
        else { ImGui.setWindowPos(menuWindowPosX, menuWindowPosY); }

        ImGui.setWindowSize(menuWindowWidth, menuWindowHeight);
        
        ImGui.text("Particle Simulator Client");
        // Update FPS and round to 2 decimal places
        fps = (float) Math.round(ImGui.getIO().getFramerate() * 100.0) / 100.0f;
        ImGui.text("FPS: " + fps);
        ImGui.text("Total time: " + (float) Math.round(ImGui.getTime() * 100.0) / 100.0f);

        if (Utils.DEBUG_MODE) {
            ImGui.text("DEBUG Mode");
            ImGui.sameLine();
            if (ImGui.button("Particle")) {
                Position particlePos = new Position(Utils.windowWidth/2, Utils.windowHeight/2);
                objectManager.addParticle(particlePos, 30.0, 10.0);
            }
        } else {
            ImGui.text("RELEASE");
            ImGui.sameLine();
            if (spawned.get()) {
                if (ImGui.button("Particle")) {
                    Position particlePos = new Position(Utils.windowWidth/2, Utils.windowHeight/2);
                    objectManager.addParticle(particlePos, 0.0, 1.0);
                }
            }
            
        }

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
            Position pos = new Position(posX.get(), posY.get());
            objectManager.spawnPlayer(pos);
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

    public float getFPS() { return fps; }
}
