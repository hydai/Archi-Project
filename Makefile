SIMULATOR = simulator
SOURCE = simulator.cpp
CC = g++
CFLAGS = -Wall

$(SIMULATOR):
	$(CC) $(SOURCE) $(CFLAGS) -o $(SIMULATOR)


.PHONY: clean
clean:
	rm *.o
