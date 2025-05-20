#ifndef PEERCHAT_SOCKET_H
#define PEERCHAT_SOCKET_H

#include "PeerChat_SocketConfig.h"

int init_server(int port);
int connect_to_peer(const char* ip, int port);
int accept_connection(int server_fd, char* client_ip, int* client_port);
int send_message(int socket_fd, const char* message);
int receive_message(int socket_fd, char* buffer, int len);
void close_socket(int socket_fd);

#endif