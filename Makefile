TARGET = locking
OBJS = main.o klock.o graph.o

CFLAGS = -Wall -g -std=c99 -Werror -pthread  -D_POSIX_C_SOURCE=199309L
CC = gcc

all: clean $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
