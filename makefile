all:
	g++ -D_PC -I test motor.cpp test/*.cpp
	./a.out
