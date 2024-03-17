package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;

/**
 * Handles all objects in the simulation.
 */
public class ObjectManager {
    private int[] textureIDs = new int[4];
    private Player player;
    private Player[] npcs = new Player[3];

    public ObjectManager() {
    }

    /**
     * Spawns a player at the given position.
     * @param position The position to spawn the player at
     * @return The player object
     */
    public Player spawnPlayer(Position position) {
        player = new Player(0, position, true);
        return player;
    }

    /***
     * Spawns a npc at the given position.
     * @param position The position to spawn the npc at
     */
    public void addNPC(Position position) {
        int npcNum = npcs.length;
        Player player = new Player(npcNum, position);
        npcs[npcNum] = player;
    }

    public void setTextureID(int index, int id) { textureIDs[index] = id; }
    public int getTextureID(int index) { return textureIDs[index]; }

    /**
     * Executes necessary object updates and rendering.
     */
    public void mainLoop(long window, double deltaTime) {
        // Player
        if (player != null) player.input(window, deltaTime);

        // Particles
        // TODO: Update particles
    }
}
