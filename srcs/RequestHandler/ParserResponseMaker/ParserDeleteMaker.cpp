/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserDeleteMaker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbarbe <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:28:00 by sbarbe            #+#    #+#             */
/*   Updated: 2025/02/13 11:28:16 by sbarbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::vector<std::string> RequestIn::DeleteResponse()
{
    std::map<int, std::string> mapCode = initCodeMap();
    std::vector<std::string> response;

    std::string tempUri = this->uri;
    
    if (tempUri == "/")
        tempUri = "";
    else
        tempUri = "." + tempUri;
    std::cout << "Tentative de suppression : " << tempUri << std::endl;

    if (!isFile(tempUri) && !isDirectory(tempUri))
        this->codeHTTP = 404; // Not Found
    else
    {
        if (isFile(tempUri))
        {
            if (std::remove(tempUri.c_str()) == 0)
                this->codeHTTP = 204; // No Content (succès)
            else
                this->codeHTTP = 403; // Forbidden (échec de suppression)
        } 
        else if (isDirectory(tempUri))
        {
            if (std::system(("rm -r " + tempUri).c_str()) == 0)
                this->codeHTTP = 204; // Succès
            else
                this->codeHTTP = 500; // Erreur serveur
        }
    }

    this->stringCode = mapCode[this->codeHTTP];

    response.push_back("HTTP/1.1 " + intToString(this->codeHTTP) + " " + this->stringCode + "\r\n");
    response.push_back("Connection: close\r\n");
    response.push_back("Date: " + getHttpDate() + "\r\n\r\n");

    return response;
}

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <cstdlib>   // Pour std::system
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

// std::vector<std::string> RequestIn::PostResponse()
// {
//     std::map<int, std::string> mapCode = initCodeMap();
//     std::vector<std::string> response;
    
//     std::string scriptPath = "./html/conect/cgi/mail/mail.py";

//     // Vérifie que le script existe
//     if (access(scriptPath.c_str(), F_OK) == -1) {
//         this->codeHTTP = 404; // Not Found
//     } else {
//         // Création d'un tube pour capter la sortie du script
//         int pipefd[2];
//         if (pipe(pipefd) == -1) {
//             this->codeHTTP = 500; // Erreur serveur
//         } else {
//             pid_t pid = fork();
//             if (pid == -1) {
//                 this->codeHTTP = 500; // Erreur serveur
//             } 
//             else if (pid == 0) { // Processus enfant (exécute `mail.py`)
//                 close(pipefd[0]); // Ferme la lecture du tube

//                 dup2(pipefd[1], STDOUT_FILENO); // Redirige stdout vers le tube
//                 close(pipefd[1]); // On n'en a plus besoin

//                 // Exécute le script CGI
//                 char *args[] = { (char*)"python3", (char*)scriptPath.c_str(), NULL };
//                 execvp(args[0], args);
                
//                 // Si `execvp` échoue
//                 exit(1);
//             } 
//             else { // Processus parent (lit la sortie du script)
//                 close(pipefd[1]); // Ferme l'écriture du tube
//                 char buffer[1024];
//                 std::string output;
//                 ssize_t bytesRead;

//                 while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
//                     buffer[bytesRead] = '\0';
//                     output += buffer;
//                 }

//                 close(pipefd[0]);
//                 waitpid(pid, NULL, 0); // Attend la fin du script

//                 this->codeHTTP = 200; // Succès
//                 response.push_back("HTTP/1.1 200 OK\r\n");
//                 response.push_back("Content-Type: text/html\r\n");
//                 response.push_back("Connection: close\r\n");
//                 response.push_back("Date: " + getHttpDate() + "\r\n\r\n");
//                 response.push_back(output); // Ajoute la sortie du script en réponse
//             }
//         }
//     }

//     this->stringCode = mapCode[this->codeHTTP];

//     if (this->codeHTTP != 200) {
//         response.clear();
//         response.push_back("HTTP/1.1 " + intToString(this->codeHTTP) + " " + this->stringCode + "\r\n");
//         response.push_back("Connection: close\r\n");
//         response.push_back("Date: " + getHttpDate() + "\r\n\r\n");
//     }

//     return response;
// }


#include <iostream>
#include <sstream>
#include <string>

std::string extractEmail(const std::string &postData) {
    size_t start = postData.find("email=");
    if (start != std::string::npos) {
        start += 6; // "email=" a 6 caractères
        size_t end = postData.find("&", start); // Trouver la fin de l'email, ou la fin de la chaîne
        if (end == std::string::npos) {
            end = postData.length(); // Si pas de '&', la fin de la chaîne est la fin de l'email
        }
        return postData.substr(start, end - start);
    }
    return ""; // Si l'email n'est pas trouvé
}

std::string extractPassword(const std::string &postData) {
    size_t start = postData.find("password=");
    if (start != std::string::npos) {
        start += 9; // "email=" a 6 caractères
        size_t end = postData.find("&", start); // Trouver la fin de l'email, ou la fin de la chaîne
        if (end == std::string::npos) {
            end = postData.length(); // Si pas de '&', la fin de la chaîne est la fin de l'email
        }
        return postData.substr(start, end - start);
    }
    return ""; // Si l'email n'est pas trouvé
}

std::string extractName(const std::string &postData) {
    size_t start = postData.find("username=");
    if (start != std::string::npos) {
        start += 9; // "email=" a 6 caractères
        size_t end = postData.find("&", start); // Trouver la fin de l'email, ou la fin de la chaîne
        if (end == std::string::npos) {
            end = postData.length(); // Si pas de '&', la fin de la chaîne est la fin de l'email
        }
        return postData.substr(start, end - start);
    }
    return ""; // Si l'email n'est pas trouvé
}

std::vector<std::string> RequestIn::PostResponse(const std::string& input)
{
    std::map<int, std::string> mapCode = initCodeMap();
    std::vector<std::string> response;

    std::string scriptPath = "./html/conect/cgi/mail/mail.py";

    if (access(scriptPath.c_str(), F_OK) == -1) {
        this->codeHTTP = 404; // Not Found
    } else {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            this->codeHTTP = 500; // Erreur serveur
        } else {
            pid_t pid = fork();
            if (pid == -1) {
                this->codeHTTP = 500; // Erreur serveur
            } 
            else if (pid == 0) { // Processus enfant (exécute `mail.py`)
                close(pipefd[0]);

                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);

                // 📧 Récupérer le nom, l'email et le mot de passe depuis la requête POST
                std::string postData = input;
                // Récupérer uniquement la partie des données de formulaire (après les en-têtes)
            size_t bodyStart = postData.find("\r\n\r\n");  // Trouver la séparation entre en-têtes et corps
            if (bodyStart != std::string::npos) {
                postData = postData.substr(bodyStart + 4);  // Extraire la partie après les en-têtes
            } else {
                 postData = "";  // Si la séparation n'a pas été trouvée
             }
                std::cout << "ici: " << std::endl;
                std::string name = extractName(postData);  // Extraire le nom
                std::string email = extractEmail(postData);  // Extraire l'email
                std::string password = extractPassword(postData);  // Extraire le mot de passe
                std::cout << name << std::endl;
                std::cout << email << std::endl;
                std::cout << password << std::endl;
                std::cout << "icic " + postData + "fin" << std::endl;
                
                setenv("REQUEST_METHOD", "POST", 1);
                std::stringstream ss;
                ss << postData.size();
                setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
                
                int inputPipe[2];
                pipe(inputPipe);
                write(inputPipe[1], postData.c_str(), postData.size());
                close(inputPipe[1]);

                dup2(inputPipe[0], STDIN_FILENO);
                close(inputPipe[0]);

                char *args[] = { (char*)"python3", (char*)scriptPath.c_str(), NULL };
                execvp(args[0], args);
                
                exit(1);
            } 
            else { // Processus parent
                close(pipefd[1]);
                char buffer[1024];
                std::string output;
                ssize_t bytesRead;

                while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytesRead] = '\0';
                    output += buffer;
                }

                close(pipefd[0]);
                waitpid(pid, NULL, 0);

                this->codeHTTP = 200;
                response.push_back("HTTP/1.1 200 OK\r\n");
                response.push_back("Content-Type: text/plain\r\n");
                response.push_back("Connection: close\r\n");
                response.push_back("Date: " + getHttpDate() + "\r\n\r\n");
                response.push_back(output);
            }
        }
    }

    return response;
}


// std::vector<std::string> RequestIn::PostResponse(const std::string& input)
// {
//     std::map<int, std::string> mapCode = initCodeMap();
//     std::vector<std::string> response;

//     std::string scriptPath = "./html/conect/cgi/mail/mail.py";

//     if (access(scriptPath.c_str(), F_OK) == -1) {
//         this->codeHTTP = 404; // Not Found
//     } else {
//         int pipefd[2];
//         if (pipe(pipefd) == -1) {
//             this->codeHTTP = 500; // Erreur serveur
//         } else {
//             pid_t pid = fork();
//             if (pid == -1) {
//                 this->codeHTTP = 500; // Erreur serveur
//             } 
//             else if (pid == 0) { // Processus enfant (exécute `mail.py`)
//                 close(pipefd[0]);

//                 dup2(pipefd[1], STDOUT_FILENO);
//                 close(pipefd[1]);

//                 // 📧 Récupérer l'email depuis la requête POST
//                 std::string postData = input;  // Ici, remplace avec les données dynamiques
//                 std::cout << "ici: " << std::endl;
//                 std::string email = extractEmail(postData);  // Extraire l'email
//                 std::cout << email << std::endl;
                
//                 setenv("REQUEST_METHOD", "POST", 1);
//                 std::stringstream ss;
//                 ss << postData.size();
//                 setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
                
//                 int inputPipe[2];
//                 pipe(inputPipe);
//                 write(inputPipe[1], postData.c_str(), postData.size());
//                 close(inputPipe[1]);

//                 dup2(inputPipe[0], STDIN_FILENO);
//                 close(inputPipe[0]);

//                 char *args[] = { (char*)"python3", (char*)scriptPath.c_str(), NULL };
//                 execvp(args[0], args);
                
//                 exit(1);
//             } 
//             else { // Processus parent
//                 close(pipefd[1]);
//                 char buffer[1024];
//                 std::string output;
//                 ssize_t bytesRead;

//                 while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
//                     buffer[bytesRead] = '\0';
//                     output += buffer;
//                 }

//                 close(pipefd[0]);
//                 waitpid(pid, NULL, 0);

//                 this->codeHTTP = 200;
//                 response.push_back("HTTP/1.1 200 OK\r\n");
//                 response.push_back("Content-Type: text/plain\r\n");
//                 response.push_back("Connection: close\r\n");
//                 response.push_back("Date: " + getHttpDate() + "\r\n\r\n");
//                 response.push_back(output);
//             }
//         }
//     }

//     return response;
// }

