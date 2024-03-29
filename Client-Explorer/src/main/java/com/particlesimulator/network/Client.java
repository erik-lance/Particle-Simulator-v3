package com.particlesimulator.network;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.objects.ObjectManager;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
// UDP Socket
import java.util.LinkedList;
import java.util.Queue;
import java.io.IOException;
import java.util.concurrent.locks.ReentrantLock;

/**
 * This communicates with the server to send and receive data.
 */
public class Client {
    private String userID;
    private ObjectManager objectManager;
    private DatagramSocket socket;
    private InetAddress address;
    private int port;
    private Thread listenerThread;
    private Thread senderThread;

    // Data Queue
    private Queue<String> sendDataQueue = new LinkedList<>();
    private ReentrantLock sendLock;

    public Client(ObjectManager objectManager) {
        this.objectManager = objectManager;
        listenerThread = new Thread(this::receiver);
        senderThread = new Thread(this::sender);
        sendLock = new ReentrantLock();

        // Connect to the server
        try {
            address = InetAddress.getByName("127.0.0.1");
            port = 5555;

            socket = new DatagramSocket();
            System.out.println("Connected to the server - " + address + ":" + port);

            // Generate userID based on time of connection
            userID = "u" + System.currentTimeMillis();

            // Set socket to blocking
            socket.setSoTimeout(0);

            listenerThread.start();
            senderThread.start();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Initial add of the player to the server
     * @param pos - Position of the player
     */
    public void addPlayerToServer(Position pos) {
        // Send data to the server
        String data = "<u>" + userID + ":" + pos.getX() + "," + pos.getY() + "</u>";

        sendLock.lock();
        sendDataQueue.add(data);
        sendLock.unlock();
    }

    /**
     * Update the player's position to the server
     * @param pos - Position of the player
     */
    public void updatePlayer(Position pos) {
        // Send data to the server
        String data = "<m>" + pos.getX() + "," + pos.getY() + "</m>";

        sendLock.lock();
        sendDataQueue.add(data);
        sendLock.unlock();
    }

    /**
     * Parses the data received from the server
     * @param data particle data request
     */
    public void parseParticleData(String data) {
        // Requests we receive from server are literally
        // the same actions the developer took to spawn
        // particles,  e.g.: batch particle method 1, 2, 3, or add particle

        // Data format to expect (add particle, batch 1, 2, 3 in order)
        // <p>x,y,angle,veloctiy</p>
        // <b>1,num,startX,endX,startY,endY,angle,velocity</b>
        // <b>2,num,startAngle,endAngle,startX,startY,velocity</b>
        // <b>3,num,startX,startY,angle,startVelocity,endVelocity</b>
        char type = data.charAt(1); // Get the type of data
        String[] values = data.substring(3, data.length() - 4).split(","); // Get the values

        if (type == 'p') {
            // Add particle
            Position pos = new Position(Float.parseFloat(values[0]), Float.parseFloat(values[1]));
            objectManager.addParticle(pos, Double.parseDouble(values[2]), Double.parseDouble(values[3]));
        } else if (type == 'b') {
            // Batch particles
            if (values[0] == "1") {
                // Batch method 1
                Position start = new Position(Float.parseFloat(values[1]), Float.parseFloat(values[2]));
                Position end = new Position(Float.parseFloat(values[3]), Float.parseFloat(values[4]));
                objectManager.batchParticleMethodOne(Integer.parseInt(values[1]), start, end, Double.parseDouble(values[6]), Double.parseDouble(values[7]));
            } else if (values[0] == "2") {
                // Batch method 2
                Position start = new Position(Float.parseFloat(values[3]), Float.parseFloat(values[4]));
                objectManager.batchParticleMethodTwo(Integer.parseInt(values[1]), Double.parseDouble(values[2]), Double.parseDouble(values[3]), start, Double.parseDouble(values[5]));
            } else if (values[0] == "3") {
                // Batch method 3
                Position start = new Position(Float.parseFloat(values[1]), Float.parseFloat(values[2]));
                objectManager.batchParticleMethodThree(Integer.parseInt(values[1]), start, Double.parseDouble(values[3]), Double.parseDouble(values[4]), Double.parseDouble(values[5]));
            }
        }
    }

    /**
     * Loads current particles in server because client joined mid-game.
     * @param record - contains ticks and particle command (e.g.:<r>23<p>particle_data</p></r>)
     */
    public void loadClient(String record) {
        // Get ticks after <r> and before next '<'
        int ticks = Integer.parseInt(record.substring(3, record.indexOf("<", 3)));

        // Get particle command. Note: Can be <p> or <b>
        String particle_command = record.substring(record.indexOf("<", 3));
        
        // Keep only the particle data
        String particle_data = particle_command.substring(3, particle_command.length() - 8);

        // Process the particle data
        parseParticleData(particle_data);

        // Update all particles in the object manager by the number of ticks
        for (int i = 0; i < ticks; i++) {
            objectManager.updateParticles();
        }
    }

    public void receiver() {
        while (true) {
            // Receive data from the server
            try {
                DatagramPacket packet = new DatagramPacket(new byte[1024], 1024, address, port);
                String data = new String(packet.getData(), 0, packet.getLength());
                // Process data
                if (data.startsWith("<p>") || data.startsWith("<b>")) {
                    parseParticleData(data);
                } else if (data.startsWith("<r>") || data.startsWith("<d>")) {
                    // This is for loading client data
                    // <r> is for receiving the player's position
                    // <d> means done loading
                    if (data.startsWith("<r>")) {
                        loadClient(data);
                    } else {
                        // TODO: Update particles by X ticks since loading
                        System.out.println("Done loading client data");
                        objectManager.clientLoaded = true;
                    }
                } else {

                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void sender() {
        while (true) {
            // Send data to the server
            try {
                // Get first data from the queue
                sendLock.lock();
                String data = sendDataQueue.poll();
                sendLock.unlock();

                if (data != null) {
                    byte[] sendData = data.getBytes();
                    DatagramPacket packet = new DatagramPacket(sendData, sendData.length, address, port);
                    socket.send(packet);
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
