#ifndef PEERCHAT_DISPLAY_H
#define PEERCHAT_DISPLAY_H

void display_help();
void display_ip(const char* ip);
void display_port(int port);
void display_connection_list();
void display_message_received(const char* sender_ip, int sender_port, const char* message);
void display_error(const char* message);

#endif