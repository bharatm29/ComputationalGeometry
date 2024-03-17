CFLAGS = -Wall -Wextra

all: out
	@ ./a

out: first_program.cpp
	g++ $(CFLAGS) -o a first_program.cpp ./src/glad.c -I./include/ -lSDL2 -ldl
