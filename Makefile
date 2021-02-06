CC = gcc

#This target can keep changing based on final binary required
TARGET = scanner

SRCDIR = src
TARGETDIR = bin

all: $(TARGET)

.PHONY: clean

clean:
	rm -rf $(TARGETDIR)