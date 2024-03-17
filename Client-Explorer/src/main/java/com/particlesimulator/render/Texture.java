package com.particlesimulator.render;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL13.GL_TEXTURE0;
import static org.lwjgl.opengl.GL13.glActiveTexture;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;

import javax.imageio.ImageIO;

import org.lwjgl.BufferUtils;

/**
 * Represents a texture in the game.
 */
public class Texture {
    private int textureId = -1;
    private int width;
    private int height;

    /**
     * Create a new texture from the given file.
     * File must be from resources/sprites
     * @param filename
     */
    public Texture(String filename) {
        BufferedImage bufferedImage;

        try {
            // Load the texture from the file
            URI file = getClass().getResource("/sprites/"+filename).toURI();
            bufferedImage = ImageIO.read(new File(file));
            width = bufferedImage.getWidth();
            height = bufferedImage.getHeight();

            // Create a new array of pixels
            int[] pixels = new int[width * height];
            bufferedImage.getRGB(0, 0, width, height, null, 0, width);

            // Create a new byte buffer to store the pixel data
            ByteBuffer buffer = BufferUtils.createByteBuffer(width * height * 4);

            // Loop through each pixel and add it to the buffer
            for (int y = 0; y < height; y++) { // Loop through each row of pixels
                for (int x = 0; x < width; x++) { // Loop through each pixel in the row
                    int pixel = pixels[y * width + x];
                    buffer.put((byte) ((pixel >> 16) & 0xFF)); // Red
                    buffer.put((byte) ((pixel >> 8) & 0xFF)); // Green
                    buffer.put((byte) (pixel & 0xFF)); // Blue
                    buffer.put((byte) ((pixel >> 24) & 0xFF)); // Alpha
                }
            }

            // Flip the buffer so it can be read
            buffer.flip();

            // Generate a new texture ID
            textureId = glGenTextures();

            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, textureId);

            // Set the texture parameters
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // Load the texture data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }   catch (IOException | URISyntaxException e) {
			e.printStackTrace();
        }
    } 

    /**
     * Clean up the texture when it is no longer needed.
     */
    @Override
    protected void finalize() throws Throwable {
        glDeleteTextures(textureId);
        super.finalize();
    }

    /**
     * Bind the texture to the given sampler.
     * @param sampler
     */
    public void bind(int sampler) {
        if (sampler >= 0 && sampler <= 31) {
            glActiveTexture(GL_TEXTURE0 + sampler);
            glBindTexture(GL_TEXTURE_2D, textureId);
        }
    }
}
