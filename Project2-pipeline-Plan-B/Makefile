SRC	= main.c
HEADER	= main.h
TARGET	= pipeline
CFLAGS	= -Wall -O0 -g
RM	= -rm -rf

all		: $(TARGET)

$(TARGET)	: $(SRC) $(HEADER)
	cc $(SRC) $(CFLAGS) -o $(TARGET)

clean		:
	$(RM) $(TARGET) $(TARGET).core
