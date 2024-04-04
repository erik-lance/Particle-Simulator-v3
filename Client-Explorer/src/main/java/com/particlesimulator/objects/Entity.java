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

        if (newX < 0) { newX = 0; }
        else if (newX > windowWidth) { newX = windowWidth; }

        if (newY < 0) { newY = 0; } 
        else if (newY > windowHeight) { newY = windowHeight; }

        position = new Position(newX, newY);

        // Round to nearest pixel
        position = new Position(Math.round(position.getX()), Math.round(position.getY()));

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

        // If own player (not NPC)
        if (isUser) { drawEdgeWalls(); }

        // Color white
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

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
     * Draws walls when the player is at the edge of the screen
     */
    public void drawEdgeWalls() {
        // Color: 42, 74, 115
        glColor4f(42f / 255f, 74f / 255f, 115f / 255f, 1.0f);

        Position roundedPos = new Position((int) position.getX(), (int) position.getY());
        

        if (roundedPos.getX() < 16) {
            // Draw left wall, width changes based on position
            glBegin(GL_QUADS);
            glVertex2f(0, 0); // Top-left
            glVertex2f((float) (entityWidth * (16 - roundedPos.getX() - 1)), 0); // Top-right
            glVertex2f((float) (entityWidth * (16 - roundedPos.getX() - 1)), windowHeight); // Bottom-right
            glVertex2f(0, windowHeight); // Bottom-left
            glEnd();
        } else if (roundedPos.getX() > windowWidth - 16) {
            // Draw right wall, width changes based on position
            glBegin(GL_QUADS);
            glVertex2f(windowWidth, 0);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f((float) (windowWidth - entityWidth * (roundedPos.getX() - (windowWidth - 15) + 1)), windowHeight);
            glVertex2f((float) (windowWidth - entityWidth * (roundedPos.getX() - (windowWidth - 15) + 1)), 0);
            glEnd();
        }
        if (roundedPos.getY() < 9) {
            // Draw top wall, height changes based on position
            glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(windowWidth, 0);
            glVertex2f((float) windowWidth, (float) (entityHeight * (9 - roundedPos.getY() - 1)));
            glVertex2f((float) 0, (float) (entityHeight * (9 - roundedPos.getY() - 1)));
            glEnd();
        } else if (roundedPos.getY() > windowHeight - 9) {
            // Draw bottom wall, height changes based on position
            glBegin(GL_QUADS);
            glVertex2f(0, windowHeight);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f((float) windowWidth, (float) (windowHeight - entityHeight * (roundedPos.getY() - (windowHeight - 8) + 1)));
            glVertex2f((float) 0, (float) (windowHeight - entityHeight * (roundedPos.getY() - (windowHeight - 8) + 1)));
            glEnd();
        }
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

            float centerX = windowWidth / 2;
            float centerY = windowHeight / 2;

            // Translate relative to player (each screen pixel is 39x38)
            int newDrawPosX = (int) (centerX + (position.getX() - playerPos.getX()) * 39);
            int newDrawPosY = (int) (centerY + (position.getY() - playerPos.getY()) * 38);

            glBegin(GL_QUADS);
            
            glTexCoord2f(0, 0);
            glVertex2f(newDrawPosX - width / 2, newDrawPosY - height / 2);

            glTexCoord2f(1, 0);
            glVertex2f(newDrawPosX + width / 2, newDrawPosY - height / 2);

            glTexCoord2f(1, 1);
            glVertex2f(newDrawPosX + width / 2, newDrawPosY + height / 2);

            glTexCoord2f(0, 1);
            glVertex2f(newDrawPosX - width / 2, newDrawPosY + height / 2);

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
        boolean withinX = Math.abs(playerPos.getX() - position.getX()) <= 33;
        boolean withinY = Math.abs(playerPos.getY() - position.getY()) <= 19;
        if (withinX && withinY) {
            this.playerPos = playerPos;
            draw();
        }
    }

    public void setClient(Client client) { this.client = client; }
    public void setDirection(Position direction) { this.curDirection = direction; }
    public void setPosition(Position position) { this.position = position; }
}
