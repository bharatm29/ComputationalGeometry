all: main

main: main.cpp verletobj.cpp simulator.cpp
	g++ -o main main.cpp -Iraylib -I../raygui -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lraylib -lGL -lm -lpthread -ldl -lrt
	./main

clean:
	rm ./main
