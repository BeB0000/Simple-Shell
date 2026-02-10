CC = gcc
CFLAGS = -Wall -g
LFLAGS = -lreadline
TARGET = shell
BIN_DIR = /usr/local/bin
SRCS = shell.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

install: $(TARGET)
	cp $(TARGET) $(BIN_DIR)
	chmod +x $(BIN_DIR)/$(TARGET)
	@echo "BeboShell installed to $(BIN_DIR)"

clean:
	rm -f $(OBJS) $(TARGET)

delete:
	rm -f $(BIN_DIR)/$(TARGET)
	@echo "BeboShell removed from $(BIN_DIR)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all install clean delete
