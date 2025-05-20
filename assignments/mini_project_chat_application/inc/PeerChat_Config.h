#ifndef PEERCHAT_CONFIG_H
#define PEERCHAT_CONFIG_H

#define MAX_CONNECTIONS 10
#define MAX_MESSAGE_LEN 100
#define MAX_IP_LEN 16
#define MAX_PORT_LEN 6

typedef struct {
    int socket_fd;
    char ip[MAX_IP_LEN];
    int port;
} Connection;

#endif