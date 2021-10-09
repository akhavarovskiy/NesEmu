cpp: src/6502.cpp include/6502.hpp
	g++ -c -g -fPIC src/6502.cpp -o lib/lib6502++.a -std=c++14 -m64 -Ofast

all: include/6502.h src/6502.c src/6502Core.c
	gcc -c -g -fPIC src/6502.c -o lib/lib6502.a -std=c99 -m64 -Ofast
