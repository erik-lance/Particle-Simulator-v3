package com.particlesimulator.network;

import com.particlesimulator.Utils.Position;
import com.particlesimulator.objects.ObjectManager;

// TCP Socket
import java.net.Socket;
import java.util.LinkedList;
import java.util.Queue;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
// Mutex
import java.util.concurrent.locks.Lock;

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
    private Queue<String> recvdataQueue = new LinkedList<>();
    private Queue<String> sendDataQueue = new LinkedList<>();

    private BufferedReader reader;
    

    public Client(ObjectManager objectManager) {
        this.objectManager = objectManager;
        listenerThread = new Thread(this::receiver);
        senderThread = new Thread(this::sender);

        // Connect to the server
        try {
            socket = new Socket("127.0.0.1", 8080);
            System.out.println("Connected to the server - " + socket.getInetAddress() + ":" + socket.getPort());

            // Generate userID based on time of connection
            userID = "u" + System.currentTimeMillis();

            // Set socket to blocking
            socket.setSoTimeout(0);

        } catch (IOException e) {
            e.printStackTrace();
        }

        listenerThread.start();
        senderThread.start();
    }

    /**
     * Initial add of the player to the server
     * @param pos - Position of the player
     */
    public void addPlayerToServer(Position pos) {
        // Send data to the server
        String data = "<n>" + userID + ":" + pos.getX() + "," + pos.getY() + "</n>";
        sendDataQueue.add(data);
    }

    /**
     * Update the player's position to the server
     * @param pos - Position of the player
     */
    public void updatePlayer(Position pos) {
        // Send data to the server
        String data = "<u>" + userID + ":" + pos.getX() + "," + pos.getY() + "</u>";
        sendDataQueue.add(data);
    }


    public void receiver() {
        while (true) {
            // Receive data from the server
        }
    }

    public void sender() {
        while (true) {
            // Send data to the server
        }
    }
}
