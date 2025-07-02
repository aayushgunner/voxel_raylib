CC = g++
CFLAGS = -lraylib -lm
TARGET = main

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) main.cpp -o $(TARGET) $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
