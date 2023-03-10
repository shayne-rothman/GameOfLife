# This makefile builds the gol executable
# to build:  make
# to cleanup: make clean
#
CC = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable

TARGET = gol
SRC = $(TARGET).c 

all: $(TARGET)

$(TARGET): $(SRC) 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET)
