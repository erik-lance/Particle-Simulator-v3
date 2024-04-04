package com.particlesimulator.network;

import com.particlesimulator.Utils;
import com.particlesimulator.Utils.Position;
import com.particlesimulator.objects.ObjectManager;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
// UDP Socket
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Objects;
import java.util.Queue;
import java.io.IOException;
import java.util.concurrent.locks.ReentrantLock;

/**
 * This communicates with the server to send and receive data.
 */
public class Client {
    private boolean running = true;
    private String userID;
    private ObjectManager objectManager;
    private DatagramSocket socket;
    private InetAddress address;
    private int port;
    private Thread listenerThread;
    private Thread senderThread;
    private Thread playerListener;

    // Data Queue
    private Queue<String> sendDataQueue = new LinkedList<>();
    private ReentrantLock sendLock;

    public Client(ObjectManager objectManager) {
        this.objectManager = objectManager;
        sendLock = new ReentrantLock();

        // Connect to the server
        try {
            address = InetAddress.getByName(Utils.SERVER_IP); // Get server IP
            port = Utils.SERVER_PORT; // Get server port

            socket = new DatagramSocket();

            // Get own ip and port
            String ownIP = socket.getLocalAddress().getHostAddress();
            int ownPort = socket.getLocalPort();

            // Generate userID by hashing the IP
            userID = "u" + ownIP.hashCode() + ownPort;
            
            System.out.println("Connected to the server - " + address + ":" + port + " as " + userID);
            System.out.println("Your address is " + ownIP + ":" + ownPort);

            

            // Set socket to blocking
            socket.setSoTimeout(0);

        } catch (IOException e) {
            e.printStackTrace();
        }

        listenerThread = new Thread(this::receiver);
        senderThread = new Thread(this::sender);

        listenerThread.start();
        senderThread.start();
    }

    /**
     * Initial add of the player to the server
     * @param pos - Position of the player
     */
    public void addPlayerToServer(Position pos) {
        System.out.println("Sending server player data");
        // Send data to the server
        String data = "<c>" + userID + ":" + pos.getX() + "," + pos.getY() + "</c>";

        sendLock.lock();
        sendDataQueue.add(data);
        sendLock.unlock();

        // Set player texture
        int lastDigit = Integer.parseInt(userID.substring(userID.length() - 1));
        objectManager.setPlayerTexutre(lastDigit);
    }

    /**
     * Update the player's position to the server
     * @param pos - Position of the player
     * @param dir - Direction of the player
     */
    public void updatePlayer(Position pos, Position dir) {
        // Send data to the server
        String data = "<m>"+ userID + "," + pos.getX() + "," + pos.getY() + "," + dir.getX() + "," + dir.getY() + "</m>";

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
        System.out.println("Parsing particle data: " + data);
        System.out.println("Type: " + type);
        System.out.println("Values: " + Arrays.toString(values));
        if (type == 'p') {
            // Add particle
            Position pos = new Position(Float.parseFloat(values[0]), Float.parseFloat(values[1]));
            System.out.println("Adding particle at " + pos.getX() + ", " + pos.getY() + " with angle " + values[2] + " and velocity " + values[3]);
            objectManager.addParticle(pos, Double.parseDouble(values[2]), Double.parseDouble(values[3]));
        } else if (type == 'b') {
            // Batch particles
            if (Objects.equals(values[0], "1")) {
                // Batch method 1
                Position start = new Position(Float.parseFloat(values[2]), Float.parseFloat(values[4]));
                Position end = new Position(Float.parseFloat(values[3]), Float.parseFloat(values[5]));
                System.out.println("BM1: Adding " + values[1] + " particles between " + start.getX() + ", " + start.getY() + " and " + end.getX() + ", " + end.getY() + " with angle " + values[6] + " and velocity " + values[7]);
                objectManager.batchParticleMethodOne(Integer.parseInt(values[1]), start, end, Double.parseDouble(values[6]), Double.parseDouble(values[7]));
            } else if (Objects.equals(values[0], "2")) {
                // Batch method 2
                Position start = new Position(Float.parseFloat(values[4]), Float.parseFloat(values[5]));
                System.out.println("BM2: Adding " + values[1] + " particles between " + values[2] + " and " + values[3] + " with angle " + values[5] + " and velocity " + values[6]);
                objectManager.batchParticleMethodTwo(Integer.parseInt(values[1]), Double.parseDouble(values[2]), Double.parseDouble(values[3]), start, Double.parseDouble(values[6]));
            } else if (Objects.equals(values[0], "3")) {
                // Batch method 3
                Position start = new Position(Float.parseFloat(values[2]), Float.parseFloat(values[3]));
                System.out.println("BM3: Adding " + values[1] + " particles at " + start.getX() + ", " + start.getY() + " with angle " + values[3] + " and velocity between " + values[4] + " and " + values[5]);
                objectManager.batchParticleMethodThree(Integer.parseInt(values[1]), start, Double.parseDouble(values[4]), Double.parseDouble(values[5]), Double.parseDouble(values[6]));
            }
        }
    }

    /**
     * Loads current particles in server because client joined mid-game.
     * @param record - contains ticks and particle command (e.g.:<r>23<p>particle_data</p></r>)
     */
    public void loadClient(String record) {
        System.out.println("Loading record: " + record);

        // Get ticks after <r> and before next '<'
        int ticks = Integer.parseInt(record.substring(3, record.indexOf("<", 3)));

        // Get particle command. Note: Can be <p> or <b>
        String particle_command = record.substring(record.indexOf("<", 3));
        
        // Keep only the particle data (with <p> or <b> tags included)
        String particle_data = particle_command.substring(0, particle_command.indexOf("</"));

        // Process the particle data
        System.out.println("Parsing particle data: " + particle_data);
        parseParticleData(particle_data);

        // Update all particles in the object manager by the number of ticks
        for (int i = 0; i < ticks; i++) {
            objectManager.updateParticles();
        }
    }

    public void receiver() {
        while (running) {
            // Receive data from the server
            try {
                DatagramPacket packet = new DatagramPacket(new byte[1024], 1024);
                socket.receive(packet);

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
                        System.out.println("Done loading client data. Added: " + objectManager.getNumParticles() + " particles."); 
                        objectManager.clientLoaded = true;
                    }
                } else if (data.startsWith("<c>")) {
                    // New NPC "<c>id:x,y</c>"
                    String[] values = data.substring(3, data.length() - 4).split(":"); // Get the values and remove tags
                    String id = values[0];
                    String[] pos = values[1].split(",");
                    Position position = new Position(Float.parseFloat(pos[0]), Float.parseFloat(pos[1]));
                    objectManager.addNPC(id, position);
                    System.out.println(id + " has joined the game at " + position.getX() + ", " + position.getY());
                    
                } else if (data.startsWith("<m>")) {
                    // Move player "<m>id,x,y,dx,dy</m>"
                    String[] values = data.substring(3, data.length() - 4).split(","); // Get the values and remove tags
                    String id = values[0];
                    Position pos = new Position(Float.parseFloat(values[1]), Float.parseFloat(values[2]));
                    Position dir = new Position(Float.parseFloat(values[3]), Float.parseFloat(values[4]));
                    objectManager.updateNPC(id, pos, dir);
                } else if (data.startsWith("<l>")) {
                    // Remove NPC "<l>id</l>"
                    String id = data.substring(3, data.length() - 4);
                    objectManager.removeNPC(id);
                } else if (data.startsWith("<u>")) {
                    // Update particle "<u>idx,x,y,angle</u>"
                    String[] values = data.substring(3, data.length() - 4).split(","); // Get the values and remove tags
                    int idx = Integer.parseInt(values[0]);
                    Position pos = new Position(Float.parseFloat(values[1]), Float.parseFloat(values[2]));
                    double angle = Double.parseDouble(values[3]);
                    objectManager.updateParticle(idx, pos, angle);
                } else {
                    // Print if data starts with a "<"
                    if (data.startsWith("<")) {
                        System.out.println(data);
                    }
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void sender() {
        while (running) {
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

    public void disconnect() {
        // Disconnect from the server
        running = false;

        // Send disconnect message
        String data = "<l>" + userID + "</l>";
        byte[] sendData = data.getBytes();
        DatagramPacket packet = new DatagramPacket(sendData, sendData.length, address, port);
        try {
            socket.send(packet);
        } catch (IOException e) {
            e.printStackTrace();
        }

        socket.close();

        // Stop threads
        listenerThread.interrupt();
        senderThread.interrupt();

        System.out.println("Disconnected from the server.");
    }
}
