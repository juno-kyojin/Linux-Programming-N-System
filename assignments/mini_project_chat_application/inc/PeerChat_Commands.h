#ifndef PEERCHAT_COMMANDS_H
#define PEERCHAT_COMMANDS_H

#include "PeerChat_Config.h"

void handle_help();
void handle_myip();
void handle_myport();
int handle_connect(const char* ip, int port);
void handle_list();
int handle_terminate(int id);
int handle_send(int id, const char* message);
void handle_exit();

#endif