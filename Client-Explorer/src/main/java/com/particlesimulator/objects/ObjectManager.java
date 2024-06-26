package com.particlesimulator.objects;

import static com.particlesimulator.Utils.DEBUG_MODE;

import java.util.ArrayList;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

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
    private ArrayList<NPC> npcs = new ArrayList<>();

    private int numParticles = 0;
    private int particlesCapacity = 1024;
    private Particle[] particles = new Particle[particlesCapacity];

    private Lock mtx;
    public boolean clientLoaded = false;

    public ObjectManager() {
        this.client = new Client(this);
        this.mtx = new ReentrantLock();
    }

    /**
     * Spawns a player at the given position.
     * @param position The position to spawn the player at
     */
    public void spawnPlayer(Position position) {
        System.out.println("Adding you to game at position: " + position.getX() + ", " + position.getY());
        player = new Player(position, true);

        // Set texture
        player.setTexture(textures[0]);
        client.addPlayerToServer(position);
        player.setClient(client);
    }

    /***
     * Spawns a npc at the given position.
     * @param id The id of the npc
     * @param position The position to spawn the npc at
     */
    public void addNPC(String id, Position position) {
        NPC npc = new NPC(id, this, position);

        // Set texture
        char last_digit = id.charAt(id.length() - 1);
        int textureNum = Character.getNumericValue(last_digit);
        npc.setTexture(textures[textureNum % 4]);

        npcs.add(npc);
    }

    public void removeNPC(String id) {
        for (int i = 0; i < npcs.size(); i++) {
            if (npcs.get(i).getUID().equals(id)) {
                mtx.lock();
                npcs.remove(i);
                mtx.unlock();
                break;
            }
        }
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

            // NPCs
            mtx.lock();
            for (int i = 0; i < npcs.size(); i++) { 
                npcs.get(i).move(deltaTime);
                npcs.get(i).draw(player.getPosition()); 
            }
            mtx.unlock();

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

            // NPCs
            mtx.lock();
            for (int i = 0; i < npcs.size(); i++) { 
                npcs.get(i).move(deltaTime);
                npcs.get(i).draw(player.getPosition()); 
            }
            mtx.unlock();

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
        for (int i = 0; i < num; i++) {
            double x = start.getX() + (end.getX() - start.getX()) * i / num;
            double y = start.getY() + (end.getY() - start.getY()) * i / num;
            System.out.println("X: " + x + " Y: " + y);
            addParticle(new Position(x, y), angle, velocity);
        }
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
        for (int i = 0; i < num; i++) {
            double angle = startAngle + (endAngle - startAngle) * i / num;
            addParticle(start, angle, velocity);
        }
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
        for (int i = 0; i < num; i++) {
            double velocity = startVelocity + (endVelocity - startVelocity) * i / num;
            addParticle(start, angle, velocity);
	    }
    }

    public void updateParticles() {
        for (int i = 0; i < numParticles; i++) {
            particles[i].update(1);
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

    /**
     * Updates NPC movement direction based on the given ID.
     * @param id The ID of the NPC to update
     */
    public void updateNPC(String id, Position pos, Position dir) {
        NPC npc = getNPCbyId(id);
        if (npc != null) { 
            npc.setDirection(dir); 
            npc.setPosition(pos);
        }
    }

    public Texture[] getTextures() { return textures; }

    /**
     * Sets the player texture based on the given number. Taken from
     * first digit of the player's unique ID.
     * @param textNum
     */
    public void setPlayerTexutre(int textNum) {
        int num = textNum % 4;
        player.setTexture(textures[num]);
    }

    public int getNumParticles() { return numParticles; }
    public NPC getNPCbyId(String id) {
        for (int i = 0; i < npcs.size(); i++) {
            if (npcs.get(i).getUID().equals(id)) {
                return npcs.get(i);
            }
        }
        return null;
    }

    public void disconnect() {
        client.disconnect();
    }

    public void updateParticle(int idx, Position pos, double angle) {
        // Check if particle exists
        if (idx < 0 || idx >= numParticles) {
            // Add a new particle
            addParticle(pos, angle, 100);
        } else {
            particles[idx].setPosition(pos);
            particles[idx].setAngle(angle);
        }
        
    }
}
