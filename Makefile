# the compiler: gcc for C program, define as g++ for C++
 CC = gcc

 # compiler flags:
 #  -g    adds debugging information to the executable file
 #  -Wall turns on most, but not all, compiler warnings
 CFLAGS  = -std=c99 -g -Wall

 # the build target executable:
 TARGET = run

 FILES = source/run.c source/fetch.c source/decode.c source/execute.c source/operations.c source/utils.c source/memacc.c source/writeback.c

 all: $(TARGET)

 $(TARGET): $(FILES)
	 $(CC) $(CFLAGS) -o $(TARGET) $(FILES)

 clean:
	 $(RM) $(TARGET)
