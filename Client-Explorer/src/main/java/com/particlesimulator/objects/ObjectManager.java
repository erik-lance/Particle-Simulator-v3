package com.particlesimulator.objects;

import static com.particlesimulator.Utils.DEBUG_MODE;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.network.Client;
import com.particlesimulator.render.Texture;

/**
 * Handles all objects in the simulation.
 */
public class ObjectManager {
    private Client client;
    private Texture[] textures = new Texture[4];
    private Player player;
    private Player[] npcs = new Player[3];

    private int numParticles = 0;
    private int particlesCapacity = 1024;
    private Particle[] particles = new Particle[particlesCapacity];
    public boolean clientLoaded = false;

    public ObjectManager() {
        this.client = new Client(this);
    }

    /**
     * Spawns a player at the given position.
     * @param position The position to spawn the player at
     * @return The player object
     */
    public Player spawnPlayer(Position position) {
        System.out.println("Adding you to game at position: " + position.getX() + ", " + position.getY());
        player = new Player(0, position, true);

        // Set texture
        player.setTexture(textures[0]);
        client.addPlayerToServer(position);

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
        if (clientLoaded) {
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
        } else if (DEBUG_MODE) {
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
     * Adds particles to the simulation using batch method one. This places
     * particles between two points in a uniform distribution.
     * @param num
     * @param start
     * @param end
     * @param angle
     * @param velocity
     */
    public void batchParticleMethodOne(int num, Position start, Position end, double angle, double velocity) {

    }

    /**
     * Adds particles to the simulation using batch method two. This places
     * particles in a uniform distribution between two angles.
     * @param num
     * @param startAngle
     * @param endAngle
     * @param start
     * @param velocity
     */
    public void batchParticleMethodTwo(int num, double startAngle, double endAngle, Position start, double velocity) {

    }

    /**
     * Adds particles to the simulation using batch method three. This places
     * particles in a uniform distribution between two velocities.
     * @param num
     * @param start
     * @param angle
     * @param startVelocity
     * @param endVelocity
     */
    public void batchParticleMethodThree(int num, Position start, double angle, double startVelocity, double endVelocity) {

    }

    public void updateParticles() {
        for (int i = 0; i < numParticles; i++) {
            particles[i].update(0.10);
        }
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
