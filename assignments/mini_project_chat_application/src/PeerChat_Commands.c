#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <netdb.h>
#include "PeerChat_Commands.h"
#include "PeerChat_Display.h"
#include "PeerChat_Socket.h"

extern Connection connections[MAX_CONNECTIONS];
extern int num_connections;
extern int server_port;

void handle_help() {
    display_help();
}

void handle_myip() {
    struct ifaddrs *ifaddr, *ifa;
    char ip[MAX_IP_LEN] = "Unknown";
    if (getifaddrs(&ifaddr) == -1) {
        display_error("Failed to get IP address");
        return;
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET) {
            continue;
        }
        if (strcmp(ifa->ifa_name, "lo") != 0) {
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip, MAX_IP_LEN, NULL, 0, NI_NUMERICHOST);
            break;
        }
    }
    freeifaddrs(ifaddr);
    display_ip(ip);
}

void handle_myport() {
    display_port(server_port);
}

int handle_connect(const char* ip, int port) {
    if (num_connections >= MAX_CONNECTIONS) {
        display_error("Maximum connections reached");
        return -1;
    }
    char local_ip[MAX_IP_LEN];
    handle_myip(); // Lấy IP local để kiểm tra tự kết nối
    // Giả định display_ip lưu IP vào biến tạm, thay bằng cách lưu trực tiếp nếu cần
    // Ở đây giả sử IP được lưu trong hàm display_ip, cần sửa lại để lấy trực tiếp
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        display_error("Failed to get IP address");
        return -1;
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET) {
            continue;
        }
        if (strcmp(ifa->ifa_name, "lo") != 0) {
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), local_ip, MAX_IP_LEN, NULL, 0, NI_NUMERICHOST);
            break;
        }
    }
    freeifaddrs(ifaddr);
    if (strcmp(ip, local_ip) == 0 && port == server_port) {
        display_error("Cannot connect to self");
        return -1;
    }
    for (int i = 0; i < num_connections; i++) {
        if (strcmp(connections[i].ip, ip) == 0 && connections[i].port == port) {
            display_error("Connection already exists");
            return -1;
        }
    }
    int socket_fd = connect_to_peer(ip, port);
    if (socket_fd == INVALID_SOCKET) {
        display_error("Failed to connect");
        return -1;
    }
    connections[num_connections].socket_fd = socket_fd;
    strncpy(connections[num_connections].ip, ip, MAX_IP_LEN);
    connections[num_connections].port = port;
    num_connections++;
    printf("Connected to %s:%d\n", ip, port);
    return 0;
}

void handle_list() {
    display_connection_list();
}

int handle_terminate(int id) {
    if (id < 1 || id > num_connections) {
        display_error("Invalid connection ID");
        return -1;
    }
    int index = id - 1;
    close_socket(connections[index].socket_fd);
    for (int i = index; i < num_connections - 1; i++) {
        connections[i] = connections[i + 1];
    }
    num_connections--;
    printf("Connection %d terminated\n", id);
    return 0;
}

int handle_send(int id, const char* message) {
    if (id < 1 || id > num_connections) {
        display_error("Invalid connection ID");
        return -1;
    }
    if (strlen(message) > MAX_MESSAGE_LEN) {
        display_error("Message too long");
        return -1;
    }
    int index = id - 1;
    if (send_message(connections[index].socket_fd, message) < 0) {
        display_error("Failed to send message");
        return -1;
    }
    printf("Message sent to connection %d\n", id);
    return 0;
}

void handle_exit() {
    for (int i = 0; i < num_connections; i++) {
        close_socket(connections[i].socket_fd);
    }
    num_connections = 0;
    exit(0);
}