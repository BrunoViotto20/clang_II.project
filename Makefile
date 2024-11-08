CC=gcc
CFLAGS=-Wall -Wextra -g
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR=includes
SRC=$(SRC_DIR)/main.c
LIB=console.c lanchonete.c
OBJ=$(addprefix $(OBJ_DIR), $(LIB:.c=.o))
EXE=$(BIN_DIR)/$(shell basename $(SRC:.c=))

# .PHONY: clean run $(OBJ) $(EXE)

all: clean setup $(EXE)

run: $(EXE)
	$<

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) $(SRC) $(shell find $(BIN_DIR)/*.o) -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $(BIN_DIR)/$(shell basename $@)

clean:
	rm -rf $(EXE) $(BIN_DIR)

setup:
	mkdir $(BIN_DIR)