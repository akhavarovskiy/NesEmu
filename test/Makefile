CC=g++
FLAGS=-g -std=c99 
INCLUDE= -I ../6502/include
LIB=-L ../6502/lib
OBJ=obj
SRC=src
OUT=nestest

nestest: $(OBJ)/nesrom.o $(OBJ)/nesbus.o $(SRC)/nestest.c
	make -C ../6502
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) -l6502  $(OBJ)/*.o $(SRC)/nestest.c -o $(OUT)

obj/nesrom.o: include/nesrom.h src/nesrom.c
	$(CC) -c $(INCLUDE) $(LIB) $(SRC)/nesrom.c -o $(OBJ)/nesrom.o

obj/nesbus.o: include/nesbus.h src/nesbus.c
	$(CC) -c $(INCLUDE) $(LIB) $(SRC)/nesbus.c -o $(OBJ)/nesbus.o

clean:
	rm $(OBJ)/*
	rm $(OUT)