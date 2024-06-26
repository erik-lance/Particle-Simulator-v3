package com.particlesimulator.objects;

import static com.particlesimulator.Utils.windowHeight;
import static com.particlesimulator.Utils.windowWidth;
import static com.particlesimulator.Utils.normalizeAngle;
import static org.lwjgl.opengl.GL11.*;

import com.particlesimulator.Utils.Position;

public class Particle {
    private int m_id;
    private Position curPosition;
    private Position oldPosition;
    private double angle;
    private double velocity;
    private int radius = 3;

    public Particle(int id, Position position, double angle, double velocity) {
        this.m_id = id;
        this.curPosition = position;
        this.oldPosition = position;
        this.velocity = velocity;

        // Fix angle
        this.angle = 360 - angle;
        if (this.angle == 360) { this.angle = 0; }

        // Convert to radians
        this.angle = Math.toRadians(this.angle);

        // Normalize the angle
        this.angle = normalizeAngle(this.angle);
    }

    /**
     * Updates the particle's position based on its velocity and angle.
     * @param deltaTime
     */
    public void update(double deltaTime) {
        double newX = curPosition.getX() + Math.cos(angle) * velocity * deltaTime;
        double newY = curPosition.getY() + Math.sin(angle) * velocity * deltaTime;
        
        if (newX < radius || newX > windowWidth - radius) {
            handleScreenCollision(newX, newY);
        } else if (newY < radius || newY > windowHeight - radius) {
            handleScreenCollision(newX, newY);
        } else {
            oldPosition = curPosition;
            curPosition = new Position(newX, newY);
        }
    }

    /**
     * Calculates the new positions of the particle after a collision with the screen.
     * @param newX The new x position (might be out of bounds)
     * @param newY The new y position (might be out of bounds)
     */
    public void handleScreenCollision(double newX, double newY) {
        // Distance from old position
        double distance = Math.sqrt(Math.pow(newX - oldPosition.getX(), 2) + Math.pow(newY - oldPosition.getY(), 2));
        distance += radius;
        
        // Calculate the new position
        if (newX <= radius || newX >= windowWidth - radius) {
            // Reflect angle
            angle = Math.PI - angle;

            // Normalize the angle
            angle = normalizeAngle(angle);

            // Move x by the distance
            newX = oldPosition.getX() + Math.cos(angle) * distance;
        } 
        
        if (newY <= radius || newY >= windowHeight - radius) {
            // Reflect angle
            angle = -angle;

            // Normalize the angle
            angle = normalizeAngle(angle);

            // Move y by the distance
            newY = oldPosition.getY() + Math.sin(angle) * distance;
        }

        oldPosition = curPosition;
        curPosition = new Position(newX, newY);
    }

    /**
     * Draws based on distance from player.
     * @param playerPosition
     */
    public void drawParticle(Position playerPosition) {
        // If within 33x19 box around the player
        if ((curPosition.getX() >= playerPosition.getX() - 16 && curPosition.getX() <= playerPosition.getX() + 16) 
        && (curPosition.getY() >= playerPosition.getY() - 9 && curPosition.getY() <= playerPosition.getY() + 9)) {
            int roundX = (int) Math.round(curPosition.getX());
            int roundY = (int) Math.round(curPosition.getY());

            // Center of screen
            double centerX = windowWidth / 2;
            double centerY = windowHeight / 2;

            // Translate relative to player (each screen pixel is 39x38)
            int newDrawPosX = (int) (centerX + (roundX - playerPosition.getX()) * 39);
            int newDrawPosY = (int) (centerY + (roundY - playerPosition.getY()) * 38);

            int height = 39;
            int width = 38;
            
            // Draw
            glColor3f(1.0f, 1.0f, 1.0f); // White
            glBegin(GL_QUADS);
            
            glVertex2d(newDrawPosX - (width/2), newDrawPosY + (height/2)); // Bottom-left
            glVertex2d(newDrawPosX + (width/2), newDrawPosY + (height/2)); // Bottom-right
            glVertex2d(newDrawPosX + (width/2), newDrawPosY - (height/2)); // Top-right
            glVertex2d(newDrawPosX - (width/2), newDrawPosY - (height/2)); // Top-left

            glEnd();
        }
    }

    public void drawDefault() {
        // Draw the particle
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glBegin(GL_QUADS);

        glVertex2d(curPosition.getX() - radius, curPosition.getY() + radius); // Bottom-left
        glVertex2d(curPosition.getX() + radius, curPosition.getY() + radius); // Bottom-right
        glVertex2d(curPosition.getX() + radius, curPosition.getY() - radius); // Top-right
        glVertex2d(curPosition.getX() - radius, curPosition.getY() - radius); // Top-left

        glEnd();
    }

    public Position getPosition() { return curPosition; }
    public int getId() { return m_id; }
    public int getRadius() { return radius; }
    public double getAngle() { return angle; }
    public double getVelocity() { return velocity; }
    public void setPosition(Position position) { curPosition = position; }
    public void setAngle(double angle) { this.angle = angle; }
}
