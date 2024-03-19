package com.particlesimulator.objects;

import static com.particlesimulator.Utils.DEBUG_MODE;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.render.Texture;

/**
 * Handles all objects in the simulation.
 */
public class ObjectManager {
    private Texture[] textures = new Texture[4];
    private Player player;
    private Player[] npcs = new Player[3];

    private int numParticles = 0;
    private int particlesCapacity = 1024;
    private Particle[] particles = new Particle[particlesCapacity];

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
        for (int i = 0; i < numParticles; i++) {
            particles[i].update(deltaTime);
            if (DEBUG_MODE) { particles[i].drawDefault(); }
            else { particles[i].drawParticle(player.getPosition()); }
        }
    }

    /**
     * Adds a particle to the simulation.
     * @param position The position to spawn the particle at
     * @param angle The angle of the particle
     * @param velocity The velocity of the particle
     */
    public void addParticle(Position position, double angle, double velocity) {
        if (numParticles >= particlesCapacity) {
            updateParticlesCapacity();
        }

        particles[numParticles] = new Particle(numParticles, position, angle, velocity);
        numParticles++;
    }

    /**
     * Dynamic array resizing for particles.
     * (Needed because we're not allowed to use libraries like ArrayList or Vector)
     */
    private void updateParticlesCapacity() {
        particlesCapacity *= 2;
        Particle[] newParticles = new Particle[particlesCapacity];
        System.arraycopy(particles, 0, newParticles, 0, numParticles); // Copy old array to new array
        particles = newParticles;
    }
}
