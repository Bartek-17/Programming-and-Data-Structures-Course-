# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c99

# Executable name
TARGET = banking_system

# Source files
SRCS = main.c account.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
