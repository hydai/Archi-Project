SIMULATOR = simulator
TARGET = single_cycle
SOURCE = simulator.cpp environment.cpp runtime.cpp parse.cpp execution.cpp dump.cpp
CC = clang++
CFLAGS = -Wall -DDEBUG

$(SIMULATOR):
	$(CC) $(SOURCE) $(CFLAGS) -o $(TARGET)


.PHONY: clean
clean:
	rm *.o
