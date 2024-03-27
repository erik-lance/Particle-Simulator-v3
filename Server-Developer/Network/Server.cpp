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

	// Generate sender threads
	for (int i = 0; i < 3; i++) { sender_threads.push_back(std::thread(&Server::sender, this)); }
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
	// If <c>x,y</c>; it is a new player connection
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
				// Generate UUID by hashing the address
				std::hash<std::string> hash_fn;
				std::string UUID = std::to_string(hash_fn(response.address));

				// Generate position based on message
				Position position;
				std::string message = response.message.substr(3);
				message = message.substr(0, message.size() - 4); // Remove the last 4 character (</c>)
				position.x = std::stoi(message.substr(0, message.find(',')));
				position.y = std::stoi(message.substr(message.find(',') + 1));

				// Generate player
				User user;
				user.address = response.address;
				user.UUID = UUID;
				user.player = object_manager->generatePlayer(UUID, position);

				// Start loading client
				// Send to client all previous particles by batch
				client_loader_threads.push_back(std::thread(&Server::clientLoader, this, user, response.message, &object_manager->getParticleHistory()));
			}
			else if (type == "<m>") {
				// Player movement update
				std::string message = response.message.substr(3);
				message = message.substr(0, message.size() - 4); // Remove the last 4 character (</m>)

				int x = std::stoi(message.substr(0, message.find(',')));
				int y = std::stoi(message.substr(message.find(',') + 1));

				// X and Y are the direction of the player
				Position direction { x, y };

				// Distribute message to all clients that are not the sender
				sendToOtherClients(response.message, response.address);
			}
			else {
				std::cout << "Unknown message type: " << type << std::endl;
				std::cout << "Message: " << response.message << std::endl;
			}
		}
	}
}

void Server::sender()
{
	while (running) {
		// Use condition variable to wait for messages
		std::unique_lock<std::mutex> lock(mtx);
		while (messages.empty()) { cv.wait(lock); }

		// Send the message
		Message message = messages.front();
		messages.pop();

		// Update the cv
		cv.notify_all();

		// Obtain host and port from dest_address of message
		std::string host = message.dest_address.substr(0, message.dest_address.find(':'));
		std::string port = message.dest_address.substr(message.dest_address.find(':') + 1);

		struct sockaddr_in client_address;
		client_address.sin_family = AF_INET;
		client_address.sin_port = htons(std::stoi(port));
		InetPtonA(AF_INET, host.c_str(), &client_address.sin_addr);

		// Send the message
		int bytes_sent = sendto(m_socket, message.message.c_str(), message.message.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));

		if (bytes_sent < 0)
		{
			// Print full error details
			char error[1024];
			strerror_s(error, sizeof(error), errno);
			std::cerr << "Error sending message: " << error << std::endl;

			// Disconnect the client by removing from the list
			for (int i = 0; i < clients.size(); i++)
			{
				if (clients[i].address == message.dest_address)
				{
					clients.erase(clients.begin() + i);
					break;
				}
			}
		}

	}
}

/**
 * This is called when a new client connects to the server. A thread is used to perform this function
 * and send the client all the particle records that have been generated so far. This comes with ticks
 * since last particle added for each record.
 * @param u The user object containing the client's address, UUID, and player object
 * @param spawn The spawn message that contains position of the player
 * @param history The particle history to send to the client
 */
void Server::clientLoader(User u, std::string spawn, std::vector<ParticleHistoryRecord>* history)
{
	// Generate the message to send to the client
	std::string address = u.address;
	std::string response = spawn;
	
	// Add all the particles to the message
	for (ParticleHistoryRecord record : *history)
	{
		std::string message = "<r>";

		// Convert record to string
		std::string ticks = std::to_string(record.ticks);
		message += ticks + record.command;

		// Add the end tag
		message += "</r>";

		// Send the message to the client
		Message msg;
		msg.dest_address = address;
		msg.message = message;

		// Add the message to the queue
		mtx.lock();
		messages.push(msg);
		mtx.unlock();
	}

	// Finally add to list of clients once caught up
	clients.push_back(u);

	// Send the message to all clients
	sendToOtherClients(response, address);
}

void Server::sendToOtherClients(std::string msg, std::string address)
{
	for (User user : clients)
	{
		if (user.address != address)
		{
			// Send the message to the sender
			Message message;
			message.dest_address = user.address;
			message.message = msg;

			// Add the message to the queue
			mtx.lock();
			messages.push(message);
			mtx.unlock();
		}
	}
}

void Server::sendToAllClients(std::string msg)
{
	for (User user : clients)
	{
		// Send the message to the sender
		Message message;
		message.dest_address = user.address;
		message.message = msg;

		// Add the message to the queue
		mtx.lock();
		messages.push(message);
		mtx.unlock();
	}
}
