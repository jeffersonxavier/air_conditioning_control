SRC_DIR = src
CLIENT_DIR = src/client
SERVER_DIR = src/server
OBJ_DIR = obj
BIN_DIR = bin

CC = g++

CFLAGS = -W -Wall -pedantic -ansi -std=c++11
INCLUDES = -Iinclude

.PHONY: clean depend

client:
	$(CC) -o $(BIN_DIR)/client $(INCLUDES) $(SRC_DIR)/*.cpp $(CLIENT_DIR)/*.cpp $(CFLAGS) -lpthread

server:
	$(CC) -o $(BIN_DIR)/server $(INCLUDES) $(SRC_DIR)/*.cpp $(SERVER_DIR)/*.cpp $(CFLAGS) -lpthread

clean:
	@echo Cleaning...
	@rm -rf obj/ bin/* *~
	@echo Done!
