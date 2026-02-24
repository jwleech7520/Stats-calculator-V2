makefileCC      = gcc
CFLAGS  = -Wall -g
LDLIBS  = -lm
TARGET  = statscalculator
INCLUDE = -I./include

SOURCES = main.c mathfunctions.c misc.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean