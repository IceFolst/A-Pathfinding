CC=g++
CFLAGS = -g -Wall -Wextra -Werror

# Target executables
TARGET1 = main

# Source files for each program
SRCS1 = main.cpp Node.cpp

# Object files for each program
OBJS1 = $(SRCS1:.cpp=.o)

# Default rule to build both programs
all: $(TARGET1)

# Rule to build the first program
$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJS1)

# Clean rule to remove all generated files
clean:
	rm -f *.o *.d $(OBJS1) $(TARGET1)

# Run each program
run1: $(TARGET1)
	./$(TARGET1)
