all: main

main: main.c
	gcc -o main main.c -Iraylib -I../raygui -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lraylib -lGL -lm -lpthread -ldl -lrt
	./main

clean:
	rm ./main
