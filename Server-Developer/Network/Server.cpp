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
	std::cout << "Shutting down server..." << std::endl;
	this->running = false;

	// Join threads
	receiver_thread.join();
	processor_thread.join();

	std::cout << "Shutting down sender threads..." << std::endl;

	// Update sender threads
	cv.notify_all();

	for (std::thread& t : sender_threads) { t.join(); }

	// Close the socket
#ifdef _WIN32
	closesocket(m_socket);
	WSACleanup();
#else
	close(m_socket);
#endif
}

/**
 * Prepares the socket and server address
 * */
void Server::init()
{
	std::cout << "Initializing server..." << std::endl;

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

	// Set to non-blocking
#ifdef _WIN32
	u_long mode = 1;
	ioctlsocket(m_socket, FIONBIO, &mode);
#else
	fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif

	// Bind the socket to the server address
	if (bind(m_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0)
	{
		// Print full error details
		char error[1024];
		strerror_s(error, sizeof(error), errno);
		std::cerr << "Error binding socket: " << error << std::endl;
		exit(1);
	}

	std::cout << "Server initialized at " << host << ":" << port << std::endl;
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
	// Receive a message
	char buffer[1024];
	struct sockaddr_in client_address;
	socklen_t client_address_length = sizeof(client_address);

	while (running) {
		// Receive a message
		int bytes_received = recvfrom(m_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_address_length);

		if (bytes_received > 0)
		{
			// std::cout << "Received message: " << buffer << std::endl;
			// Get string of the client address host:port
			char client_host[NI_MAXHOST];
			char client_service[NI_MAXSERV];

			// Get client address
			getnameinfo((struct sockaddr*)&client_address, client_address_length, client_host, NI_MAXHOST, client_service, NI_MAXSERV, 0);

			// Transform address to string
			inet_ntop(AF_INET, &client_address.sin_addr, client_host, NI_MAXHOST);

			// Add the message to the responses queue
			mtx.lock();

			Response response;
			response.address = std::string(client_host) + ":" + std::string(client_service);
			response.message = std::string(buffer, bytes_received);

			// Add the message to the queue
			responses.push(response);

			// std::cout << "["+ response.address +"] Added: " << response.message << " to queue" << std::endl;

			mtx.unlock();
		}
		else
		{
			// Print full error details if not empty
			/*char error[1024];
			strerror_s(error, sizeof(error), errno);
			std::cerr << "Error receiving message: " << error << std::endl;*/
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
		if (responses.size() > 0)
		{
			mtx.lock();
			Response response = responses.front();
			responses.pop();
			mtx.unlock();

			// Process the message
			// std::cout << "MSG: " << response.message << std::endl;

			// Check message type by first three (3) characters
			std::string type = response.message.substr(0, 3);

			if (type == "<c>") {
				std::cout << "["+ response.address +"] New player connection: " << response.message << std::endl;
				// New player connection "<c>UUID:x,y</c>"
				// Get UUID from inner message before ':' character
				std::string UUID = response.message.substr(3);
				UUID = UUID.substr(0, UUID.find(':'));

				// Generate position based on message
				Position position;
				std::string message = response.message.substr(3);
				message = message.substr(message.find(':') + 1);
				message = message.substr(0, message.size() - 4); // Remove the last 4 character (</c>)

				position.x = std::stoi(message.substr(0, message.find(',')));
				position.y = std::stoi(message.substr(message.find(',') + 1));

				// Generate player
				User user;
				user.address = response.address;
				user.UUID = UUID;
				
				object_manager->generatePlayer(UUID, position);

				// Start loading client
				// Send to client all previous particles by batch

				// We take a pointer to the history of the particles because the history is updated in real-time
				// To avoid '&' requires I-value error, we use a pointer to the history
				std::vector<ParticleHistoryRecord> history = object_manager->getParticleHistory();
				client_loader_threads.push_back(std::thread(&Server::clientLoader, this, user, response.message, history));
			}
			else if (type == "<m>") {
				// Player movement update (Contains player pos and direction)
				// "<m>UID,x,y,dirX,dirY</m>"
				std::cout << "["+ response.address +"] Player movement: " << response.message << std::endl;
				std::string message = response.message.substr(3);
				message = message.substr(0, message.size() - 4); // Remove the last 4 character (</m>)

				// First get indices of all the commas
				std::vector<int> indices;
				for (int i = 0; i < message.size(); i++)
				{
					if (message[i] == ',') { indices.push_back(i); }
				}

				// Get UID
				std::string UID = message.substr(0, indices[0]);

				// Get the player x and y
				int x = std::stoi(message.substr(indices[0] + 1, indices[1] - indices[0] - 1));
				int y = std::stoi(message.substr(indices[1] + 1, indices[2] - indices[1] - 1));

				// Get the direction of the player
				int dirX = std::stoi(message.substr(indices[2] + 1, indices[3] - indices[2] - 1));
				int dirY = std::stoi(message.substr(indices[3] + 1));


				// X and Y are the direction of the player
				Position position = { x, y };
				Position direction = { dirX, dirY };

				// Distribute message to all clients that are not the sender
				sendToOtherClients(response.message, response.address);

				// Get player's index based on address and client list
				int index = -1;
				for (int i = 0; i < clients.size(); i++)
				{
					if (clients[i].address == response.address)
					{
						index = i;
						break;
					}
				}

				// Update player's position and direction
				if (index != -1)
				{
					object_manager->updatePlayerMovement(index, position, direction);
				}
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
		while (messages.empty() && running) { cv.wait(lock); }

		if (!running) { return; }

		// Send the message
		Message message = messages.front();
		messages.pop();

		// Update the cv
		cv.notify_all();

		// Obtain host and port from dest_address of message
		std::string host = message.dest_address.substr(0, message.dest_address.find(':'));
		std::string port = message.dest_address.substr(message.dest_address.find(':') + 1);

		std::cout << "Sending message to " << host << ":" << port << std::endl;

		struct sockaddr_in client_address;
		client_address.sin_family = AF_INET;
		client_address.sin_port = htons(std::stoi(port));
		InetPtonA(AF_INET, host.c_str(), &client_address.sin_addr);

		// Send the message
		int bytes_sent = sendto(m_socket, message.message.c_str(), message.message.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));

		if (bytes_sent < 0)
		{
			int error_code = WSAGetLastError();
			std::cout << "Error sending message: " << error_code << std::endl;

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
void Server::clientLoader(User u, std::string spawn, std::vector<ParticleHistoryRecord> history)
{
	std::cout << "["+  u.address + "] Loading in player: " + u.UUID << std::endl;
	std::cout << "Current history size: " << history.size() << std::endl;

	// Generate the message to send to the client
	std::string address = u.address;
	std::string response = spawn;

	// Add all the particles to the message
	for (ParticleHistoryRecord record : history)
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
		cv.notify_one();
		mtx.unlock();
	}

	// Send "DONE" message to client
	Message done = { address, "<d></d>" };

	// Add the message to the queue
	mtx.lock();
	messages.push(done);
	cv.notify_one();
	mtx.unlock();

	// Send the message to all clients
	sendToOtherClients(response, address);

	// Finally add to list of clients once caught up
	clients.push_back(u);

	std::cout << "[" + u.address + "] Player loaded: " + u.UUID << std::endl;
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
			cv.notify_one();
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
		cv.notify_one();
		mtx.unlock();
	}
}
