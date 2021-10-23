cpp: src/6502.cpp include/6502.hpp
	g++ -shared -c -g -fPIC src/6502.cpp -std=c++14 -m64 -O3 -o lib/lib6502++.o

cpp-wtest:
	g++ -shared -c -g -fPIC src/6502.cpp -std=c++14 -m64 -O3 -o lib/lib6502++.o -D MOS6502_UNIT_TEST

all: include/6502.h src/6502.c src/6502Core.c
	gcc -c -g -fPIC src/6502.c -std=c99 -m64 -O3 -o lib/lib6502.a

test-cpp: cpp-wtest test/6502.cpp
	g++ -Wall -fPIC lib/lib6502++.o test/6502.cpp -m64 -I./include -L./lib -lgtest -lpthread -o test-cpp -D MOS6502_UNIT_TEST
	./test-cpp
