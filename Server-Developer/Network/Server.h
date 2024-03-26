#pragma once
#include "../Objects/ObjectManager.h"
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <errno.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

struct Message {
	std::string dest_address;
	std::string message;
};

struct Response {
	std::string address;
	std::string message;
};

struct User {
	std::string address;
	std::string UUID;
	Player* player;
};

class Server {
public:
	Server(std::string host, int port, ObjectManager* object_manager);
	~Server();
	void init();
	void start();
	void receiver();
	void processor();
	void sender();

	void sendToOtherClients(std::string msg, std::string address);
	void sendToAllClients(std::string msg);
private:
	std::string host;
	int port;
	std::mutex mtx; // to lock the messages queue
	std::condition_variable cv;

	ObjectManager* object_manager;
	SOCKET m_socket;
	struct sockaddr_in server_address;

	// Connections
	std::vector<User> clients; // Contains UUID,host:port,player

	// Queue
	std::queue<Message> messages; // to send
	std::queue <Response> responses; // received

	// Threads
	bool running = true;
	std::thread receiver_thread;
	std::thread processor_thread;
	std::vector<std::thread> sender_threads;
};