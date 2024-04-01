package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;

import static org.lwjgl.glfw.GLFW.*;

public class Player extends Entity {
    public Player(Position pos, boolean isUser) {
        super(pos, isUser);
    }
    
    public Player(Position pos) {
        super(pos);
    }

    /**
     * Get's player input and updates the player's position
     */
    public void input(long window, double deltaTime) {
        Position direction = new Position(0, 0);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            direction = direction.add(new Position(0, -1));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            direction = direction.add(new Position(0, 1));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            direction = direction.add(new Position(-1, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            direction = direction.add(new Position(1, 0));
        }
        
        move(direction, deltaTime);
    }
}
