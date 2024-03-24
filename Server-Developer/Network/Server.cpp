#include "Server.h"

Server::Server(std::string host, int port, ObjectManager* object_manager)
{
	this->host = host;
	this->port = port;
	this->object_manager = object_manager;

	// If windows, initialize winsock
	#ifdef _WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		{
			std::cerr << "Error initializing Winsock" << std::endl;
			exit(1);
		}
	#endif

	init();
}

Server::~Server()
{
	this->running = false;
}

/**
 * Prepares the socket and server address
 * */
void Server::init()
{
	// Create a socket
	this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(1);
	}

	// Set up the server address
	memset((char*)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	InetPtonA(AF_INET, host.c_str(), &server_address.sin_addr); // Convert the host address to a usable format

	// Set socket to reuse address
	int opt = 1;
	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) != 0)
	{
		// Print full error details
		char error[1024];
		strerror_s(error, sizeof(error), errno);
		std::cerr << "Error setting socket options: " << error << std::endl;
		exit(1);
	}

	// Set socket to non-blocking

	// Bind the socket to the server address
	if (bind(m_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0)
	{
		// Print full error details
		char error[1024];
		strerror_s(error, sizeof(error), errno);
		std::cerr << "Error binding socket: " << error << std::endl;
		exit(1);
	}
}

void Server::start()
{
	// Start threads
	receiver_thread = std::thread(&Server::receiver, this);
	processor_thread = std::thread(&Server::processor, this);
}

void Server::receiver()
{
	while (running) {

	}
}

void Server::processor()
{
	while (running) {

	}
}
