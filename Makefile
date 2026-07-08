CC = gcc
CFLAGS = -Wall -std=c99 

SRC = *.c
TARGET = engine

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

clear:
	rm -f $(TARGET)
