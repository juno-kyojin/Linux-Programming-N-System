#include <stdio.h>
#include "PeerChat_Display.h"
#include "PeerChat_Config.h"

extern Connection connections[MAX_CONNECTIONS];
extern int num_connections;

void display_help() {
    printf("Available commands:\n");
    printf("  help        : Display this help message\n");
    printf("  myip        : Display IP address\n");
    printf("  myport      : Display listening port\n");
    printf("  connect <ip> <port> : Connect to another peer\n");
    printf("  list        : List all connections\n");
    printf("  terminate <id> : Terminate a connection\n");
    printf("  send <id> <message> : Send a message to a connection\n");
    printf("  exit        : Close all connections and exit\n");
}

void display_ip(const char* ip) {
    printf("IP address: %s\n", ip);
}

void display_port(int port) {
    printf("Listening port: %d\n", port);
}

void display_connection_list() {
    if (num_connections == 0) {
        printf("No active connections.\n");
        return;
    }
    printf("ID  IP Address       Port\n");
    printf("--  ---------------  -----\n");
    for (int i = 0; i < num_connections; i++) {
        printf("%-2d  %-15s  %d\n", i + 1, connections[i].ip, connections[i].port);
    }
}

void display_message_received(const char* sender_ip, int sender_port, const char* message) {
    printf("Message received from %s:%d\n", sender_ip, sender_port);
    printf("Message: \"%s\"\n", message);
}

void display_error(const char* message) {
    printf("Error: %s\n", message);
}