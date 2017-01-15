CC = g++
CFLAGS = -Wall -Wextra -O2 -std=c++11 -pipe -DONLINE_JUDGE -stdlib=libc++
OBJECTS = *.o

all: main

# Compiles the factory executable and runs it with default test case
main : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS)

# Compiles all c files in to .o files
%.o : %.cpp
	$(CC) $(CFLAGS) -c $<
# check memory leaks for default test case
check:
	valgrind --tool=memcheck --leak-check=yes ./a.out
test:
	./a.out
# clean all .o files
clean:
	rm -rf *.o *~