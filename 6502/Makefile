cpp: src/6502.cpp include/6502.hpp
	g++ -c -g -fPIC src/6502.cpp -std=c++14 -m64 -O3 -o lib/lib6502++.o
#	ld -shared lib/lib6502++.o -o lib/lib6502++.so 


all: include/6502.h src/6502.c src/6502Core.c
	gcc -c -g -fPIC src/6502.c -std=c99 -m64 -O3 -o lib/lib6502.a

test-cpp: cpp test/6502.cpp
	g++ -Wall -fPIC lib/lib6502++.o test/6502.cpp -m64 -I./include -L./lib -lgtest -lpthread -o test-cpp
	./test-cpp
