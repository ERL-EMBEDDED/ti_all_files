CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -pthread

SRCDIR = src
OBJDIR = obj
BINDIR = bin

# List your source files explicitly
SRCS = $(SRCDIR)/tcpip_client.c $(SRCDIR)/can_communication.c $(SRCDIR)/main.c
# Create object file names by replacing '.c' with '.o'
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
TARGET = $(BINDIR)/tcpip_can

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Compiling $(TARGET) from $(OBJS)"
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule to compile each source file into an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "Compiling $@ from $<"
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean

