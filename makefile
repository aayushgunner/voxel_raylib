CC = gcc
CFLAGS = -lraylib -lm
TARGET = main

all: $(TARGET)

$(TARGET): main.c
	$(CC) main.c -o $(TARGET) $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
