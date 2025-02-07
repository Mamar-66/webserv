#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_CLIENTS 1000
#define MAX_THREADS 500

std::atomic<int> active_clients(0);
std::atomic<int> successful_clients(0);
std::mutex print_mutex;

// Fonction pour afficher l'état en temps réel
void display_status()
{
	while (active_clients > 0)
	{
		std::lock_guard<std::mutex> lock(print_mutex);
		std::cout << "\rClients actifs : " << active_clients
				  << " | Connexions réussies : " << successful_clients
				  << "     " << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

// Fonction pour simuler un client HTTP
void client_simulation(int client_id)
{
	active_clients++;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::cerr << "[Client " << client_id << "] Erreur de création du socket\n";
		active_clients--;
		return;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "[Client " << client_id << "] Échec de la connexion\n";
		close(sock);
		active_clients--;
		return;
	}

	successful_clients++;

	std::string http_request = "GET / HTTP/1.1\r\nHost: " + std::string(SERVER_IP) + "\r\n\r\n";
	send(sock, http_request.c_str(), http_request.size(), 0);

	char buffer[2048] = {0};
	recv(sock, buffer, sizeof(buffer), 0);

	std::lock_guard<std::mutex> lock(print_mutex);
	std::cout << "[Client " << client_id << "] Réponse : " << buffer << "\n";

	close(sock);
	active_clients--;
	if (successful_clients >= 1000)
		std::cout << "attend join thread" << std::endl;
}

int main()
{
	std::vector<std::thread> clients;
	std::thread status_thread(display_status);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		while (active_clients >= MAX_THREADS)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		clients.push_back(std::thread(client_simulation, i + 1));
	}

	for (auto &client : clients)
	{
		client.join();
	}

	status_thread.join();

	std::cout << "\nTous les clients ont terminé.\n";
	std::cout << "Total de connexions réussies : " << successful_clients << "\n";

	return 0;
}
