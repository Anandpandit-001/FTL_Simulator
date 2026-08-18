CC = gcc
CFLAGS = -Iinc -Wall -Wextra -g

SHELL := cmd.exe

BIN_DIR = bin
TARGET = bin/main
SRCS = $(wildcard src/*.c)

$(BIN_DIR):
ifeq ($(OS),Windows_NT)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
else
	mkdir -p $(BIN_DIR)
endif

# convert src/*.c to bin/*.o
OBJS = $(patsubst src/%.c, bin/%.o, $(SRCS))

.PHONY: all clean run

all: $(BIN_DIR) $(TARGET)
ifeq ($(OS),Windows_NT)
	@if exist $(BIN_DIR)\*.o del /q $(BIN_DIR)\*.o
else
	rm -f $(BIN_DIR)/*.o
endif

# link all object files to execution files
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# compile src/ .c to bin/ .o
bin/%.o: src/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(BIN_DIR)\* del /q $(BIN_DIR)\*
else
	rm -f $(BIN_DIR)/*
endif


run: all
ifeq ($(OS),Windows_NT)
	.\$(BIN_DIR)\main.exe
else
	./$(TARGET)
endif