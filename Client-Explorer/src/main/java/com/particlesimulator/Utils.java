package com.particlesimulator;

import static org.lwjgl.opengl.ARBTextureRectangle.GL_TEXTURE_RECTANGLE_ARB;
import static org.lwjgl.opengl.GL11.*;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import org.lwjgl.BufferUtils;

public class Utils {
    private Utils() {
        // Prevent instantiation
    }

    public static int windowWidth = 1280;
    public static int windowHeight = 720;

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
    }

    /**
     * Load a texture from a file
     * @param path The path to the file
     * @return The texture ID
     */
    public static int glLoadTexture(String path) {
        int texture = glGenTextures();
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);

        // Get the texture from the file
        InputStream in = null;
        try {
            in = new FileInputStream(path);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        int width = 39;
        int height = 38;

        // Load the texture data
        ByteBuffer buffer = BufferUtils.createByteBuffer(width * height * 4);
        byte[] data = new byte[width * height * 4];
        try {
            in.read(data);
        } catch (IOException e) {
            e.printStackTrace();
        }
        buffer.put(data);
        buffer.flip();

        // Set the parameters
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Upload the texture data
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        return texture;
    }

}
