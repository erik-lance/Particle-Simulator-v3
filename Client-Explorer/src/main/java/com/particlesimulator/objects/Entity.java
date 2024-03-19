package com.particlesimulator.objects;

import static com.particlesimulator.Utils.windowHeight;
import static com.particlesimulator.Utils.windowWidth;

import static org.lwjgl.opengl.GL11.*;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.render.Texture;



/**
 * This class represents an entity in the simulation. Used
 * by Players and helps with rendering.
 */
public abstract class Entity {
    private Position position;
    private Texture texture;
    private boolean isUser;
    private float speed = 10.0f;
    
    public Entity(Position pos, boolean isUser) {
        this.position = pos;
        this.isUser = isUser;
    }

    public Entity(Position pos) {
        this.position = pos;
        this.isUser = false;
    }

    public void move(Position direction, double deltaTime) { 
        double newX = position.getX() + (direction.getX() * deltaTime * speed);
        double newY = position.getY() + (direction.getY() * deltaTime * speed);

        if (newX < 0) { newX = 0; }
        else if (newX > windowWidth) { newX = windowWidth; }

        if (newY < 0) { newY = 0; } 
        else if (newY > windowHeight) { newY = windowHeight; }

        position = new Position(newX, newY);
    }

    public Position getPosition() { return this.position; }

    public void setTexture(Texture texture) { this.texture = texture; }
    public Texture getTexture() { return this.texture; }

    public void draw() {
        if (texture == null) {
            throw new IllegalStateException("Texture not set for entity");
        }

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture.getId());

        prepareQuad();
    }

    /**
     * Prepares the quad for rendering based on position. Draws
     * conditionally on whether the entity is a user or not.
     */
    private void prepareQuad() {
        glBegin(GL_QUADS);
        
        // Draw quad at the center
        if (isUser) {
            float width = texture.getWidth();
            float height = texture.getHeight();

            float x = (float) position.getX();
            float y = (float) position.getY();

            glBegin(GL_QUADS);

            glTexCoord2f(0, 0);
            glVertex2f(x, y);

            glTexCoord2f(1, 0);
            glVertex2f(x + width, y);

            glTexCoord2f(1, 1);
            glVertex2f(x + width, y + height);

            glTexCoord2f(0, 1);
            glVertex2f(x, y + height);

            glEnd();
        } else {
            /** TODO: Conditional drawing based on player position
             *  if within the 33 x 19 grid requirement.
            */
        }

        glEnd();
    }
}
