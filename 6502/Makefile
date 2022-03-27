cpp: src/6502.cc include/6502.hpp
	clang++ -c -g -fPIC src/6502.cc -std=c++14 -m64 -O3 -o lib/lib6502++.so

cpp-wtest: src/6502.cc include/6502.hpp
	clang++ -c -g -fPIC src/6502.cc -std=c++14 -m64 -O3 -o lib/lib6502++.so -D MOS6502_UNIT_TEST

test-cpp: cpp-wtest test/am.cc test/official/adc.cc test/official/asl.cc test/official/lsr.cc test/main.cc 
	clang++ -Wall -fPIC lib/lib6502++.so \
	test/am.cc \
	test/official/adc.cc test/official/asl.cc \
	test/official/lsr.cc \
	test/main.cc \
	-m64 -I./include -L./lib -lgtest -lpthread -o test-cpp -D MOS6502_UNIT_TEST
	./test-cpp
