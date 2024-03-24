#pragma once
#include "../Objects/ObjectManager.h"
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
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

class Server {
public:
	Server(std::string host, int port, ObjectManager* object_manager);
	~Server();
	void init();
	void start();
	void receiver();
	void processor();
private:
	std::string host;
	int port;
	std::mutex mtx; // to lock the messages queue

	ObjectManager* object_manager;
	SOCKET m_socket;
	struct sockaddr_in server_address;

	// Connections
	std::vector<SOCKET> clients;

	// Queue
	std::queue<std::string> messages; // to send
	std::queue <std::string> responses; // received

	// Threads
	bool running = true;
	std::thread receiver_thread;
	std::thread processor_thread;
};