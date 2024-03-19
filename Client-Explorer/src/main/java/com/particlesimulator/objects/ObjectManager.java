package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.render.Texture;

/**
 * Handles all objects in the simulation.
 */
public class ObjectManager {
    private Texture[] textures = new Texture[4];
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

        // Set texture
        player.setTexture(textures[0]);

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
    
    public void loadTextures() {
        textures[0] = new Texture("1.png");
        textures[1] = new Texture("2.png");
        textures[2] = new Texture("3.png");
        textures[3] = new Texture("4.png");
    }

    /**
     * Executes necessary object updates and rendering.
     */
    public void mainLoop(long window, double deltaTime) {
        // Player
        if (player != null) {
            player.input(window, deltaTime);
            player.draw();
        }

        // Particles
        // TODO: Update particles
    }
}
