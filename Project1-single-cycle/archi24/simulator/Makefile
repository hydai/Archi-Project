SIMULATOR = simulator
TARGET = single_cycle
SOURCE = main.cpp \
		 simulator.cpp \
		 execute.cpp RTypeExecution.cpp ITypeExecution.cpp JTypeExecution.cpp \
		 decode.cpp
CC = clang++
CFLAGS = -Wall 

all::$(SIMULATOR)

$(SIMULATOR):
	$(CC) $(SOURCE) $(CFLAGS) -o $(TARGET)

db:
	$(CC) $(SOURCE) -Wall -g -o $(TARGET)


.PHONY: clean
clean:
	rm $(TARGET)
