
INCLUDE    = 6502/include
LIB        = 6502/lib
BUILD_NAME = NesEmu
OBJECTS    = 
FLAGS      = -std=c++11 -m64 -Ofast -g

$(BUILD_NAME): include/main.h src/main.c obj/rom.o obj/bus.o obj/map.o obj/ppu.o obj/jpd.o 
	make -C 6502
	g++ src/main.c obj/rom.o obj/bus.o obj/map.o obj/ppu.o  obj/jpd.o  -I $(INCLUDE) -L $(LIB) -l6502 -lSDL2 -lpthread -o $(BUILD_NAME) $(FLAGS)

obj/rom.o: include/rom.h src/rom.c
	g++ -c -I $(INCLUDE) -c src/rom.c -o obj/rom.o $(FLAGS)

obj/bus.o: include/bus.h src/bus.c
	g++ -c -I $(INCLUDE) -c src/bus.c -o obj/bus.o $(FLAGS)

obj/map.o: include/map.h src/map.c
	g++ -c -I $(INCLUDE) -c src/map.c -o obj/map.o $(FLAGS)

obj/ppu.o: include/ppu.h src/ppu.cpp
	g++ -c -I $(INCLUDE) -c src/ppu.cpp -o obj/ppu.o $(FLAGS)

obj/jpd.o: include/jpd.h src/jpd.c
	g++ -c -I $(INCLUDE) -c src/jpd.c   -o obj/jpd.o $(FLAGS)

clean:
	rm obj/*
	rm $(BUILD_NAME)
