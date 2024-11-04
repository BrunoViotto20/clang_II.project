CC=gcc
CFLAGS=-Wall -Wextra -g
SRC=main.c
LIB=
OBJ=$(LIB:.c=.o)
EXE=$(SRC:.c=)

.PHONY: clean run $(EXE) $(OBJ)

all: clean $(EXE)

run: $(EXE)
	./$<

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(SRC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE) $(OBJ)
