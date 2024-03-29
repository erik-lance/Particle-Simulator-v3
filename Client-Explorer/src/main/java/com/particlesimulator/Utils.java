package com.particlesimulator;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL12.GL_CLAMP_TO_EDGE;

import java.io.IOException;
import java.nio.ByteBuffer;
import org.lwjgl.stb.STBImage;

public class Utils {
    private Utils() {
        // Prevent instantiation
    }

    public static int windowWidth = 1280;
    public static int windowHeight = 720;
    public static boolean DEBUG_MODE = true;

    public static final String SERVER_IP = "127.0.0.1";
    public static final int SERVER_PORT = 8888;

    // Records
    public record Position(double x, double y) { 
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Position position = (Position) o;
            return Double.compare(position.x, x) == 0 && Double.compare(position.y, y) == 0;
        }

        public Position add(Position pos) {
            return new Position(x + pos.x, y + pos.y);
        }

        public double getX() { return x; }
        public double getY() { return y; }
    }

    /**
     * Load a texture from a file
     * @param filename The path to the file
     * @return The texture ID
     */
    public static int glLoadTexture(String filename) {
        int[] width = new int[1];
        int[] height = new int[1];
        int[] channels = new int[1];

        ByteBuffer imageBuffer;
        
        String filepath = Utils.class.getResource("/sprites/"+filename).getFile();
        System.out.println(filepath);

        // Remove the leading slash
        if (filepath.startsWith("/")) {
            filepath = filepath.substring(1);
        }

        try {
            // Load image data using STBImage
            
            imageBuffer = STBImage.stbi_load(filepath, width, height, channels, 4);
            if (imageBuffer == null) {
                throw new IOException("Failed to load image: " + filename);
            }
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }

        int textureId = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[0], height[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

        // Free image data
        STBImage.stbi_image_free(imageBuffer);

        return textureId;
    }

    /**
     * Normalize an angle to be between 0 and 2pi
     * @param angle
     * @return
     */
    public static double normalizeAngle(double angle) {
        return angle % (2 * Math.PI);
    }
}
