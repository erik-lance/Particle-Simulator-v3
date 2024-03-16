package com.particlesimulator;

import org.lwjgl.Version;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL;

import static org.lwjgl.opengl.GL11.*;


public class Window {
    private int width, height;
    private String title;
    private long glfwWindow;

    private static Window window = null;

    private Window() {
        this.width = 1289;
        this.height = 720;
        this.title = "Particle Simulator - Client";
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
    }

    /**
     * Main application loop for the client's
     * rendering and input handling
     */
    public void loop() {
        // Run the rendering loop until the user has attempted to close
        // the window or has pressed the ESCAPE key.
        while (!GLFW.glfwWindowShouldClose(glfwWindow)) {
            // Set clear color
            glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            GLFW.glfwSwapBuffers(glfwWindow);

            // Poll for window events. The key callback above will only be
            // invoked during this call.
            GLFW.glfwPollEvents();
        }
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
