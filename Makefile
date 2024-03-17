CFLAGS = -Wall -Wextra

ct: colored_program.out
	@ ./colored_program


colored_program.out: colored_triangle.cpp
	g++ $(CFLAGS) -o colored_program colored_triangle.cpp ./src/glad.c -I./include/ -lSDL2 -ldl

sp: shader_program.out
	@ ./shader_program


shader_program.out: shaders.cpp
	g++ $(CFLAGS) -o shader_program shaders.cpp ./src/glad.c -I./include/ -lSDL2 -ldl

fg: first_program.out
	@ ./first_program


first_program.out: first_program.cpp
	g++ $(CFLAGS) -o first_program first_program.cpp ./src/glad.c -I./include/ -lSDL2 -ldl

clean:
	rm ./first_program
	rm ./shader_program
	rm ./colored_program

help:
	@ echo -e "fg -> First Program\nsp -> Shader Program\nct -> Colored Triangle\n"
