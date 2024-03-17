package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;

/**
 * Handles all objects in the simulation.
 */
public class ObjectManager {
    private int[] textureIDs = new int[4];

    public ObjectManager() {
    }

    /**
     * Spawns a player at the given position.
     * @param position The position to spawn the player at
     * @return The player object
     */
    public Player spawnPlayer(Position position) {
        Player player = new Player();
        player.setPosition(position);
        return player;
    }

    public void setTextureID(int index, int id) { textureIDs[index] = id; }
    public int getTextureID(int index) { return textureIDs[index]; }
}
