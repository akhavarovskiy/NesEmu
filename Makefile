
INCLUDE    = 6502/include
LIB        = 6502/lib
BUILD_NAME = NesEmu
OBJECTS    = 
FLAGS      = -std=c++14 -m64 -Ofast -g

$(BUILD_NAME): obj/ricoh2A0X.o obj/bus.o obj/interrupts.o obj/rom.o obj/mapper0.o 
	make -C 6502
	clang++ src/main.cc obj/ricoh2A0X.o obj/interrupts.o obj/rom.o obj/mapper0.o obj/bus.o -I$(INCLUDE) -L$(LIB) -l6502++ -lSDL2 -lpthread -o $(BUILD_NAME) $(FLAGS)

obj/ricoh2A0X.o: include/ricoh2A0X.hpp src/ricoh2A0X.cc
	clang++ -c -I $(INCLUDE) -c src/ricoh2A0X.cc -o obj/ricoh2A0X.o $(FLAGS)

obj/bus.o: include/bus.hpp src/bus.cc
	clang++ -c -I $(INCLUDE) -c src/bus.cc -o obj/bus.o $(FLAGS)

obj/interrupts.o: include/interrupts.hpp src/interrupts.cc
	clang++ -c -I $(INCLUDE) -c src/interrupts.cc -o obj/interrupts.o $(FLAGS)

obj/mapper0.o: include/mappers/mapper0.hpp src/mappers/mapper0.cc 
	clang++ -c -I $(INCLUDE) -c src/mappers/mapper0.cc -o obj/mapper0.o $(FLAGS)

obj/rom.o: include/rom.hpp src/rom.cc
	clang++ -c -I $(INCLUDE) -c src/rom.cc -o obj/rom.o $(FLAGS)

clean:
	rm obj/*
	rm $(BUILD_NAME)
