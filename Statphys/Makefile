all: main

main: main.o Demonstration.o DemonstrationState.o State.o
	g++ main.o Demonstration.o DemonstrationState.o State.o -o result -std=c++17 -lpthread -lsfml-system -lsfml-window -lsfml-graphics

main.o: main.cpp
	g++ main.cpp -std=c++17 -lpthread -lsfml-system -lsfml-window -lsfml-graphics -c

DemonstrationState.o: DemonstrationState.cpp
	g++ DemonstrationState.cpp -std=c++17 -lpthread -lsfml-system -lsfml-window -lsfml-graphics -c

Demonstration.o: Demonstration.cpp
	g++ Demonstration.cpp -std=c++17 -lpthread -lsfml-system -lsfml-window -lsfml-graphics -c

State.o: State.cpp
	g++ State.cpp -std=c++17 -lpthread -lsfml-system -lsfml-window -lsfml-graphics -c

clean:
	-rm *.o *.gcno *.gcda *.info result
