SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CC = g++

CFLAGS = -W -Wall -pedantic -ansi -std=c++11 -MMD -g3
INCLUDES = -Iinclude

CLIENT = $(BIN_DIR)/client

SRC = ${wildcard $(SRC_DIR)/*.cpp}
OBJ = ${addprefix $(OBJ_DIR)/, ${notdir ${SRC:.cpp=.o}}} 

.PHONY: clean depend

all:
	@mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
	$(MAKE) $(CLIENT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Building $@
	@$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CLIENT): $(OBJ)
	@echo Building executable...
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ) $(LIBS)
	@echo Done!

clean:
	@echo Cleaning...
	@rm -rf obj/ bin/ *~
	@echo Done!

-include $(OBJ:.o=.d)
