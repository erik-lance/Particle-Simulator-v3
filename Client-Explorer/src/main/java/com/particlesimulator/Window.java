package com.particlesimulator;

import org.lwjgl.Version;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL;

import com.particlesimulator.objects.ObjectManager;
import com.particlesimulator.render.Texture;

import static org.lwjgl.opengl.GL11.*;


public class Window {
    private int width, height;
    private String title;
    private long glfwWindow;

    private static Window window = null;
    private GUI gui;

    private ObjectManager objectManager;

    private Window() {
        this.width = Utils.windowWidth;
        this.height = Utils.windowHeight;
        this.title = "Particle Simulator - Client";

        objectManager = new ObjectManager();
    }

    public static Window get() {
        if (Window.window == null) {
            Window.window = new Window();
        }
        return Window.window;
    }

    /**
     * Run the application
     */
    public void run() {
        System.out.println("Using LWJGL " + Version.getVersion() + "!");
        init();
        loop();
        
        shutdown();
    }

    /**
     * Initialize the window and the OpenGL context
     */
    public void init() {
        // Initialize GLFW
        if (!GLFW.glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        // Configure GLFW
        GLFW.glfwDefaultWindowHints(); // optional, the current window hints are already the default
        GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE); // the window will stay hidden after creation
        GLFW.glfwWindowHint(GLFW.GLFW_RESIZABLE, GLFW.GLFW_TRUE); // the window will be resizable

        // Create the window
        glfwWindow = GLFW.glfwCreateWindow(width, height, title, 0, 0);
        if (glfwWindow == 0) {
            throw new IllegalStateException("Failed to create window");
        }

        // Make the OpenGL context current
        GLFW.glfwMakeContextCurrent(glfwWindow);
        // Enable v-sync
        GLFW.glfwSwapInterval(1);

        // Make the window visible
        GLFW.glfwShowWindow(glfwWindow);

        // This line is critical for LWJGL's interoperation with GLFW's
        // OpenGL context, or any context that is managed externally.
        GL.createCapabilities();

        // Prepares the window for 2D rendering
        glEnable2D();

        gui = new GUI(glfwWindow, objectManager);
    }

    /**
     * Main application loop for the client's
     * rendering and input handling
     */
    public void loop() {
        // Run the rendering loop until the user has attempted to close
        // the window or has pressed the ESCAPE key.

        double deltaTime = 0.0;

        while (!GLFW.glfwWindowShouldClose(glfwWindow)) {
            double time = GLFW.glfwGetTime();
            deltaTime = time - deltaTime;
            deltaTime = deltaTime > 0.1 ? 0.1 : deltaTime;

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update the object manager
            objectManager.mainLoop(glfwWindow, deltaTime);

            // Update ImGui
            gui.newFrame();
            gui.update();
            gui.render();

            // Swap buffers and poll events
            GLFW.glfwSwapBuffers(glfwWindow);
            GLFW.glfwPollEvents();
        }
    }

    /**
     * Enable 2D primitive rendering by setting up appropriate
     * orthographic projection and matrices 
     */
    private void glEnable2D() {
        // Set viewport
        glViewport(0, 0, width, height);

        // Save a copy of the projection matrix so that we can restore it
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        // Set up orthographic projection
        glOrtho(0, width, height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Disable depth testing
        glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        // Disable dithering
        glDisable(GL_DITHER);

        // Disable blending
        glDisable(GL_BLEND);

        // Prepare textures
        objectManager.loadTextures();
    }

    /**
     * Free the window callbacks and destroy the window
     */
    public void shutdown() {
        GLFW.glfwSetWindowShouldClose(glfwWindow, true);
        GLFW.glfwDestroyWindow(glfwWindow);
        GLFW.glfwTerminate();

        // Set the window to null
        Window.window = null;
    }
}
