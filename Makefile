# Makefile for Fibonacci Cache Project

# Object files and shared libraries
OBJS = main.o fibonacci.o cache.o
LIBS = libmemoize.so

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi -std=c11 -I/opt/homebrew/include
LDFLAGS = -ldl -lgmp -L/opt/homebrew/lib

# Default target
all: main $(LIBS)

# Rule to create shared library (.so) from C source file
lib%.so: %.c cache.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build the main executable
main: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)

# Dependencies
main.o: fibonacci.h cache.h
fibonacci.o: fibonacci.h

# Clean build artifacts
clean:
	rm -f main $(OBJS) $(LIBS)
