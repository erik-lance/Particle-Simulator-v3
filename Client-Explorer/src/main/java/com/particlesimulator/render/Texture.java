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

import com.particlesimulator.Utils;

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
        textureId = Utils.glLoadTexture(filename);

        // Default size in the 33x19 grid of sprites
        width = 39;
        height = 38;
    } 

    /**
     * Clean up the texture when it is no longer needed.
     */
    @Override
    protected void finalize() throws Throwable {
        glDeleteTextures(textureId);
        super.finalize();
    }
    
    public int getId() { return textureId; }
    public int getWidth() { return width; }
    public int getHeight() { return height; }

    public void updateTexture(String filename) {
        glDeleteTextures(textureId);
        textureId = Utils.glLoadTexture(filename);
    }
}
