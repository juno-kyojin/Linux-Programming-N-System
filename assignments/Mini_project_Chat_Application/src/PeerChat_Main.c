#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "PeerChat_Config.h"
#include "PeerChat_Socket.h"
#include "PeerChat_Commands.h"
#include "PeerChat_Display.h"

Connection connections[MAX_CONNECTIONS];
int num_connections = 0;
int server_port = 0;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    server_port = atoi(argv[1]);
    if (server_port <= 0 || server_port > 65535) {
        fprintf(stderr, "Invalid port number\n");
        return 1;
    }
    int server_fd = init_server(server_port);
    if (server_fd == INVALID_SOCKET) {
        return 1;
    }
    fd_set read_fds;
    char buffer[MAX_MESSAGE_LEN + 1];
    printf("Chat application started. Type 'help' for commands.\n");
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(server_fd, &read_fds);
        int max_fd = server_fd;
        for (int i = 0; i < num_connections; i++) {
            FD_SET(connections[i].socket_fd, &read_fds);
            if (connections[i].socket_fd > max_fd) {
                max_fd = connections[i].socket_fd;
            }
        }
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select failed");
            continue;
        }
        if (FD_ISSET(server_fd, &read_fds)) {
            char client_ip[MAX_IP_LEN];
            int client_port;
            int client_fd = accept_connection(server_fd, client_ip, &client_port);
            if (client_fd != INVALID_SOCKET && num_connections < MAX_CONNECTIONS) {
                connections[num_connections].socket_fd = client_fd;
                strncpy(connections[num_connections].ip, client_ip, MAX_IP_LEN);
                connections[num_connections].port = client_port;
                num_connections++;
                printf("New connection from %s:%d\n", client_ip, client_port);
            } else if (client_fd != INVALID_SOCKET) {
                close_socket(client_fd);
            }
        }
        for (int i = 0; i < num_connections; i++) {
            if (FD_ISSET(connections[i].socket_fd, &read_fds)) {
                int bytes = receive_message(connections[i].socket_fd, buffer, MAX_MESSAGE_LEN);
                if (bytes <= 0) {
                    printf("Connection %d (%s:%d) closed\n", i + 1, connections[i].ip, connections[i].port);
                    close_socket(connections[i].socket_fd);
                    for (int j = i; j < num_connections - 1; j++) {
                        connections[j] = connections[j + 1];
                    }
                    num_connections--;
                    i--;
                } else {
                    display_message_received(connections[i].ip, connections[i].port, buffer);
                }
            }
        }
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char input[256];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                continue;
            }
            input[strcspn(input, "\n")] = '\0';
            char* command = strtok(input, " ");
            if (command == NULL) {
                continue;
            }
            if (strcmp(command, "help") == 0) {
                handle_help();
            } else if (strcmp(command, "myip") == 0) {
                handle_myip();
            } else if (strcmp(command, "myport") == 0) {
                handle_myport();
            } else if (strcmp(command, "connect") == 0) {
                char* ip = strtok(NULL, " ");
                char* port_str = strtok(NULL, " ");
                if (ip == NULL || port_str == NULL) {
                    display_error("Usage: connect <ip> <port>");
                } else {
                    int port = atoi(port_str);
                    if (port <= 0 || port > 65535) {
                        display_error("Invalid port number");
                    } else {
                        handle_connect(ip, port);
                    }
                }
            } else if (strcmp(command, "list") == 0) {
                handle_list();
            } else if (strcmp(command, "terminate") == 0) {
                char* id_str = strtok(NULL, " ");
                if (id_str == NULL) {
                    display_error("Usage: terminate <id>");
                } else {
                    int id = atoi(id_str);
                    handle_terminate(id);
                }
            } else if (strcmp(command, "send") == 0) {
                char* id_str = strtok(NULL, " ");
                char* message = strtok(NULL, "");
                if (id_str == NULL || message == NULL) {
                    display_error("Usage: send <id> <message>");
                } else {
                    int id = atoi(id_str);
                    handle_send(id, message);
                }
            } else if (strcmp(command, "exit") == 0) {
                handle_exit();
            } else {
                display_error("Unknown command");
            }
        }
    }
    close_socket(server_fd);
    return 0;
}