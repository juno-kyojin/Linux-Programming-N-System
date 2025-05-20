CC = gcc
CFLAGS = -Wall
SRC_DIR = src
INC_DIR = inc
BIN_DIR = bin
SOURCES = $(SRC_DIR)/PeerChat_Main.c $(SRC_DIR)/PeerChat_Socket.c $(SRC_DIR)/PeerChat_Commands.c $(SRC_DIR)/PeerChat_Display.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = $(BIN_DIR)/PeerChat

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(BIN_DIR)

.PHONY: all clean