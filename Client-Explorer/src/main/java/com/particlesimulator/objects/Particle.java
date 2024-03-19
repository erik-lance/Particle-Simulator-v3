package com.particlesimulator.objects;

import static com.particlesimulator.Utils.windowHeight;
import static com.particlesimulator.Utils.windowWidth;
import static com.particlesimulator.Utils.normalizeAngle;

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
        this.angle = angle;
        this.velocity = velocity;
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
}
