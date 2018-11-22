# the compiler: gcc for C program, define as g++ for C++
 CC = gcc

 # compiler flags:
 #  -g    adds debugging information to the executable file
 #  -Wall turns on most, but not all, compiler warnings
 CFLAGS  = -std=c99 -g -Wall

 # the build target executable:
 TARGET = run

 FILES = code/run.c code/fetch.c code/decode.c code/execute.c code/operations.c

 all: $(TARGET)

 $(TARGET): code/$(TARGET).c
	 $(CC) $(CFLAGS) -o $(TARGET) $(FILES)

 clean:
	 $(RM) $(TARGET)
