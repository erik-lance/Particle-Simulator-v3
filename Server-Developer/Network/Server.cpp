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
	// Create a socket (UDP)
	this->m_socket = socket(AF_INET, SOCK_DGRAM, 0);
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
		// Receive a message
		char buffer[1024];
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);

		int bytes_received = recvfrom(m_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_address_length);

		if (bytes_received > 0)
		{
			// Get string of the client address host:port
			char client_host[NI_MAXHOST];
			char client_service[NI_MAXSERV];
			getnameinfo((struct sockaddr*)&client_address, client_address_length, client_host, NI_MAXHOST, client_service, NI_MAXSERV, 0);

			// Add the message to the responses queue
			mtx.lock();

			Response response;
			response.address = std::string(client_host) + ":" + std::string(client_service);
			response.message = std::string(buffer, bytes_received);

			// Add the message to the queue
			responses.push(response);

			mtx.unlock();
		}
		else
		{
			// Print full error details
			char error[1024];
			strerror_s(error, sizeof(error), errno);
			std::cerr << "Error receiving message: " << error << std::endl;
		}
	}
}

/**
 * Processes player movement and other messages
 */
void Server::processor()
{
	// If <c>DATETIME</c>; it is a new player connection
	// If <m>x,y</m>; it is a player movement direction
	while (running) {
		if (messages.size() > 0)
		{
			mtx.lock();
			Response response = responses.front();
			responses.pop();
			mtx.unlock();

			// Process the message
			std::cout << "MSG: " << response.message << std::endl;

			// Check message type by first three (3) characters
			std::string type = response.message.substr(0, 3);

			if (type == "<c>") {
				// New player connection
				// Add address to the clients list
				clients.push_back(response.address);

				// TODO: Send to client all previous particles by batch
			}
			else if (type == "<m>") {
				// Player movement update
				std::string message = response.message.substr(3);
				message = message.substr(0, message.size() - 4); // Remove the last 4 character (</m>)

				int x = std::stoi(message.substr(0, message.find(',')));
				int y = std::stoi(message.substr(message.find(',') + 1));

				// X and Y are the direction of the player
				Position direction { x, y };

			}
			else {
				std::cout << "Unknown message type: " << type << std::endl;
				std::cout << "Message: " << response.message << std::endl;
			}
		}
	}
}
