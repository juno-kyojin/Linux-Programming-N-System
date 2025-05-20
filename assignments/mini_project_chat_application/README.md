# PeerChat: TCP Socket-Based Chat Application

PeerChat is a peer-to-peer chat application built using TCP sockets, enabling message exchange between remote peers. It integrates both client and server functionalities in a single program, supports up to 10 simultaneous connections, and provides a command-line interface (CLI) for user interaction.

## Features

- Command-line interface for intuitive user interaction
- Display IP address and listening port of the application
- Establish and manage TCP connections between peers
- Support up to 10 concurrent connections
- List, terminate, and send messages over connections
- Handle errors (invalid IPs, self-connections, duplicate connections)
- Messages up to 100 characters

## Requirements

- GCC compiler (version 7.5 or higher recommended)
- Make utility
- Linux or Unix-like environment (tested on Ubuntu 20.04)
- Standard C libraries (sys/socket.h, netinet/in.h, arpa/inet.h)

## Installation

Ensure GCC and Make are installed:

```bash
sudo apt update
sudo apt install build-essential
```

Clone or extract the project to your local machine:

```bash
cd ~/path/to/Mini_project_Chat_Application
```

## Build Instructions

Navigate to the project directory:

```bash
cd Mini_project_Chat_Application
```

Build the application:

```bash
make all
```

This creates the executable `bin/PeerChat`.

(Optional) Clean build files:

```bash
make clean
```

## Run Instructions

Run the application with a port number (1–65535):

```bash
./bin/PeerChat <port>
```

Example:

```bash
./bin/PeerChat 4323
```

To test peer-to-peer communication:

1. Open multiple terminals (or use multiple machines).
2. Run the application with different ports in each terminal:

```bash
./bin/PeerChat 4322  # Terminal 1
./bin/PeerChat 4323  # Terminal 2
./bin/PeerChat 4324  # Terminal 3
```

3. Use commands to connect and exchange messages (see below).

## Commands

The application provides the following commands via the CLI:

### `help`
Display available commands and their descriptions.

```
help
```

### `myip`
Display the IP address of this application (non-loopback, e.g., not 127.0.0.1).

```
myip
```

### `myport`
Display the port this application is listening on.

```
myport
```

### `connect <ip> <port>`
Establish a TCP connection to a peer at the specified IP and port.

```
connect 127.0.0.1 4322
```

Errors: Invalid IP, self-connection, duplicate connection.

### `list`
Display a numbered list of all active connections (ID, IP, port).

```
list
```

### `terminate <id>`
Terminate the connection with the specified ID (from list).

```
terminate 1
```

Error: Invalid ID.

### `send <id> <message>`
Send a message (up to 100 characters) to the connection with the specified ID.

```
send 1 Hello from 4323
```

Errors: Invalid ID, message too long.

### `exit`
Close all connections and terminate the application.

```
exit
```

## Example Usage

**Terminal 1 (peer 1):**
```
./bin/PeerChat 4322

Chat application started. Type 'help' for commands.
```

**Terminal 2 (peer 2):**
```
./bin/PeerChat 4323

Chat application started. Type 'help' for commands.
connect 127.0.0.1 4322

Connected to 127.0.0.1:4322
```

**Terminal 2:**
```
list

ID  IP Address       Port
--  ---------------  -----
1   127.0.0.1        4322

send 1 Hello from 4323

Message sent to connection 1
```

**Terminal 1:**
```
Message received from 127.0.0.1:4323
Message: "Hello from 4323"

terminate 1

Connection 1 terminated
```

**Terminal 2:**
```
Connection 1 (127.0.0.1:4322) closed
list

No active connections.
```

## Project Structure

```
Mini_project_Chat_Application/
├── Makefile
├── README.md
├── bin
│   └── PeerChat
├── inc
│   ├── PeerChat_Config.h
│   ├── PeerChat_Commands.h
│   ├── PeerChat_SocketConfig.h
│   ├── PeerChat_Socket.h
│   └── PeerChat_Display.h
└── src
    ├── PeerChat_Commands.c
    ├── PeerChat_Main.c
    ├── PeerChat_Socket.c
    ├── PeerChat_Display.c
    └── (object files: *.o)
```

