#include <iostream>
#include <thread>
#include <vector>
#include <cstring>      // Pour memset()
#include <sys/socket.h> // Pour socket()
#include <netinet/in.h> // Pour sockaddr_in
#include <arpa/inet.h>  // Pour inet_pton()
#include <unistd.h>     // Pour close()

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define NUM_CLIENTS 100 // Nombre de clients à simuler

// Fonction pour simuler un client HTTP
void client_simulation(int client_id) {
    // Création du socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[Client " << client_id << "] Erreur de création du socket\n";
        return;
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connexion au serveur
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[Client " << client_id << "] Échec de la connexion au serveur\n";
        close(sock);
        return;
    }

    std::cout << "[Client " << client_id << "] Connecté au serveur !\n";

    // Création de la requête HTTP
    std::string http_request = "GET / HTTP/1.1\r\n";
    http_request += "Host: " + std::string(SERVER_IP) + "\r\n";
    http_request += "Connection: close\r\n\r\n";

    // Envoi de la requête HTTP
    send(sock, http_request.c_str(), http_request.size(), 0);

    // Réception de la réponse du serveur
    char buffer[9999] = {0};
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        std::cout << "[Client " << client_id << "] Réponse du serveur :\n" << buffer << "\n";
    } else {
        std::cout << "[Client " << client_id << "] Aucune réponse reçue\n";
    }

    // Fermeture de la connexion
    close(sock);
    std::cout << "[Client " << client_id << "] Déconnecté\n";
}

int main() {
    std::vector<std::thread> clients;

    // Lancer plusieurs clients en parallèle
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        clients.push_back(std::thread(client_simulation, i + 1));
    }

    // Attendre que tous les clients terminent
    for (auto& client : clients) {
        client.join();
    }

    std::cout << "Tous les clients ont terminé leur exécution.\n";
    return 0;
}
