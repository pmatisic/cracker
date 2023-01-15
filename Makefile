CC      := gcc
LDFLAGS := -lssl -lcrypto -lpthread
TARGET  := hashcrack

SOURCES := hash.c main.c

all: $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	@ rm $(TARGET)
