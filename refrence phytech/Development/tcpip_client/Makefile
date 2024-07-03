CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -pthread

SRCDIR = src
OBJDIR = obj
BINDIR = bin

# List your source files
SRCS = $(wildcard $(SRCDIR)/*.c)
# Create object file names by replacing '.c' with '.o'
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
TARGET = $(BINDIR)/my_program

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule to compile each source file into an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean

