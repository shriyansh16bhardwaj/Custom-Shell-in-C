# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = custom_shell
SRCS = MT24090_PA01.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
.PHONY: all clean
