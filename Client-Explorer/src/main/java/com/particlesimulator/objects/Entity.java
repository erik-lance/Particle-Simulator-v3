package com.particlesimulator.objects;

import static com.particlesimulator.Utils.DEBUG_MODE;
import static com.particlesimulator.Utils.windowHeight;
import static com.particlesimulator.Utils.windowWidth;

import static org.lwjgl.opengl.GL11.*;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.network.Client;
import com.particlesimulator.render.Texture;



/**
 * This class represents an entity in the simulation. Used
 * by Players and helps with rendering.
 */
public abstract class Entity {
    private Client client;
    private Position position;
    private Texture texture;
    private boolean isUser;
    private float speed = 100.0f;
    private int entityWidth = 39;
    private int entityHeight = 38;
    private Position oldDirection = new Position(0, 0);
    private Position curDirection = new Position(0, 0);
    private Position playerPos = new Position(0, 0);
    
    public Entity(Position pos, boolean isUser) {
        this.position = pos;
        this.isUser = isUser;
    }

    public Entity(Position pos) {
        this.position = pos;
        this.isUser = false;
    }

    public void move(Position direction, double deltaTime) { 
        oldDirection = curDirection;
        curDirection = direction;

        double newX = position.getX() + (direction.getX() * deltaTime * speed);
        double newY = position.getY() + (direction.getY() * deltaTime * speed);

        if (newX < (entityWidth/2)) { newX = (entityWidth/2); }
        else if (newX > windowWidth - (entityWidth/2)) { newX = windowWidth - (entityWidth/2); }

        if (newY < (entityHeight/2)) { newY = (entityHeight/2); } 
        else if (newY > windowHeight - (entityHeight/2)) { newY = windowHeight - (entityHeight/2); }

        position = new Position(newX, newY);

        // If client is not null and direction has changed, send the new position and direction
        if (client != null && !oldDirection.equals(curDirection) && isUser) {
            client.updatePlayer(position, curDirection);
        }
    }

    /**
     * Called by NPCs to move based on currently held direction
     * @param deltaTime - Time since last frame
     */
    public void move(double deltaTime) {
        move(curDirection, deltaTime);
    }

    public Position getPosition() { return this.position; }

    public void setTexture(Texture texture) { this.texture = texture; }
    public Texture getTexture() { return this.texture; }

    public void draw() {
        if (texture == null) {
            throw new IllegalStateException("Texture not set for entity");
        }

        glEnable(GL_TEXTURE_2D);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture.getId());

        // Remove the white background
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        prepareQuad();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }

    /**
     * Prepares the quad for rendering based on position. Draws
     * conditionally on whether the entity is a user or not.
     */
    private void prepareQuad() {
        glBegin(GL_QUADS);
        
        // Draw quad at the center
        if (DEBUG_MODE) {
            float width = texture.getWidth();
            float height = texture.getHeight();

            float x = (float) position.getX();
            float y = (float) position.getY();

            glBegin(GL_QUADS);

            glTexCoord2f(0, 0); // Top-left
            glVertex2f(x - width / 2, y - height / 2);

            glTexCoord2f(1, 0);
            glVertex2f(x + width / 2, y - height / 2);

            glTexCoord2f(1, 1);
            glVertex2f(x + width / 2, y + height / 2);

            glTexCoord2f(0, 1);
            glVertex2f(x - width / 2, y + height / 2);

            glEnd();
        } else if (!isUser) {
            // Draw the NPC relative to the player
            float width = texture.getWidth();
            float height = texture.getHeight();

            float x = (float) playerPos.getX() - (float) position.getX();
            float y = (float) playerPos.getY() - (float) position.getY();

            glBegin(GL_QUADS);

            glTexCoord2f(0, 0);
            glVertex2f(x - width / 2, y - height / 2);

            glTexCoord2f(1, 0);
            glVertex2f(x + width / 2, y - height / 2);

            glTexCoord2f(1, 1);
            glVertex2f(x + width / 2, y + height / 2);

            glTexCoord2f(0, 1);
            glVertex2f(x - width / 2, y + height / 2);

            glEnd();

        } else if (isUser) {
            // Draw the user at center of screen
            float width = texture.getWidth();
            float height = texture.getHeight();

            float x = (float) windowWidth / 2 - width / 2;
            float y = (float) windowHeight / 2 - height / 2;

            glBegin(GL_QUADS);
            
            glTexCoord2f(0, 0);
            glVertex2f(x - width / 2, y - height / 2);

            glTexCoord2f(1, 0);
            glVertex2f(x + width / 2, y - height / 2);

            glTexCoord2f(1, 1);
            glVertex2f(x + width / 2, y + height / 2);

            glTexCoord2f(0, 1);
            glVertex2f(x - width / 2, y + height / 2);

            glEnd();
        }

        glEnd();
    }

    /**
     * This draw is called by NPC to see if they should be drawn
     * @param playerPos - Position of the player
     */
    public void draw(Position playerPos) {
        // Check if within 33x19 pixels of the player
        if (Math.abs(playerPos.getX() - position.getX()) < 33 && Math.abs(playerPos.getY() - position.getY()) < 19) {
            this.playerPos = playerPos;
            draw();
        }
    }

    public void setClient(Client client) { this.client = client; }
    public void setDirection(Position direction) { this.curDirection = direction; }
    public void setPosition(Position position) { this.position = position; }
}
