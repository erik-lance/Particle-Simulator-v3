package com.particlesimulator.network;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.objects.ObjectManager;

// TCP Socket
import java.net.Socket;
import java.util.LinkedList;
import java.util.Queue;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.concurrent.locks.ReentrantLock;

/**
 * This communicates with the server to send and receive data.
 */
public class Client {
    private String userID;
    private ObjectManager objectManager;
    private Socket socket;
    private Thread listenerThread;
    private Thread senderThread;

    // Data Queue
    private Queue<String> sendDataQueue = new LinkedList<>();

    private BufferedReader reader;
    private BufferedWriter writer;
    private ReentrantLock sendLock;
    

    public Client(ObjectManager objectManager) {
        this.objectManager = objectManager;
        listenerThread = new Thread(this::receiver);
        senderThread = new Thread(this::sender);
        sendLock = new ReentrantLock();

        // Connect to the server
        try {
            socket = new Socket("127.0.0.1", 8080);
            System.out.println("Connected to the server - " + socket.getInetAddress() + ":" + socket.getPort());

            // Generate userID based on time of connection
            userID = "u" + System.currentTimeMillis();

            // Set socket to blocking
            socket.setSoTimeout(0);

            // Prepare reader-writer
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

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
        // <b>1,startX,endX,startY,endY,angle,velocity</b>
        // <b>2,startAngle,endAngle,startX,startY,velocity</b>
        // <b>3,startX,startY,angle,startVelocity,endVelocity</b>
    }

    public void receiver() {
        while (true) {
            // Receive data from the server
            try {
                String data = reader.readLine();
                while (data != null) {
                    // Process data
                    if (data.startsWith("<p>") || data.startsWith("<b>")) {
                        parseParticleData(data);
                    } else {
                        System.out.println(data);
                    }

                    data = reader.readLine(); // Read next line
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void sender() {
        while (true) {
            // Send data to the server
            if (!sendDataQueue.isEmpty()) {
                try {
                    // Get first data from the queue
                    sendLock.lock();
                    String data = sendDataQueue.poll();
                    sendLock.unlock();

                    writer.write(data); // Send data to socket by writing to the buffer
                    writer.flush(); // Flush the buffer to send the data
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
