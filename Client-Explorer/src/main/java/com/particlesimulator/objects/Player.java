package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;
import org.lwjgl.glfw.*;

import static org.lwjgl.opengl.GL11.*;

public class Player {
    private Position position = new Position(0, 0);
    private int textureId = -1;
    private int playerId = 0;


    public Player() {
    }

    /**
     * Sets the texture id of the player based
     * on openGL's texture handling
     */
    public void setTexture(int textureId) {
        this.textureId = textureId;
    }
    /**
     * Draws the player at the center of the screen
     */
    private void drawCenter() {
        // Uses openGL to draw the player
        glColor3f(1.0f, 1.0f, 1.0f); // White color

        // Load the texture
        // Bind the texture
        // Draw the texture
    }

    public void setPosition(Position position) { this.position = position; }
}
