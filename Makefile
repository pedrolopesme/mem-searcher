CC = clang
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =

TARGET = memory_searcher
SOURCES = main.c reader.c scanner.c writer.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
