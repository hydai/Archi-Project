SIMULATOR = simulator
TARGET = pipeline
SOURCE = main.cpp \
		 simulator.cpp 
CC = clang++
CFLAGS = -Wall 
DBFLAGS = -g
all::$(SIMULATOR)

$(SIMULATOR):
	$(CC) $(SOURCE) $(CFLAGS) -o $(TARGET)

db:
	$(CC) $(SOURCE) $(DBFLAGS) -o $(TARGET)


.PHONY: clean
clean:
	rm $(TARGET)
