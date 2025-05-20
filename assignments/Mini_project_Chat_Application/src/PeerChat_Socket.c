#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "PeerChat_Socket.h"
#include "PeerChat_Config.h"

int init_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        perror("Socket creation failed");
        return INVALID_SOCKET;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return INVALID_SOCKET;
    }
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        close(server_fd);
        return INVALID_SOCKET;
    }
    return server_fd;
}

int connect_to_peer(const char* ip, int port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        perror("Socket creation failed");
        return INVALID_SOCKET;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
        close(socket_fd);
        return INVALID_SOCKET;
    }
    if (connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Connection failed");
        close(socket_fd);
        return INVALID_SOCKET;
    }
    return socket_fd;
}

int accept_connection(int server_fd, char* client_ip, int* client_port) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return INVALID_SOCKET;
    }
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, MAX_IP_LEN);
    *client_port = ntohs(client_addr.sin_port);
    return client_fd;
}

int send_message(int socket_fd, const char* message) {
    if (send(socket_fd, message, strlen(message), 0) < 0) {
        perror("Send failed");
        return -1;
    }
    return 0;
}

int receive_message(int socket_fd, char* buffer, int len) {
    int bytes = recv(socket_fd, buffer, len - 1, 0);
    if (bytes <= 0) {
        return bytes;
    }
    buffer[bytes] = '\0';
    return bytes;
}

void close_socket(int socket_fd) {
    close(socket_fd);
}