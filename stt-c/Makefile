CC_RELEASE=g++ -Wall -O4 -pedantic -std=c++11 -DNDEBUG

all: bin/greedy_stt bin/greedy_stt_debug

bin/greedy_stt: greedy_stt.cpp parse_input.o
	mkdir -p bin
	$(CC_RELEASE) greedy_stt.cpp parse_input.o -o bin/greedy_stt

bin/greedy_stt_var%: greedy_stt.cpp parse_input.o
	mkdir -p bin
	$(CC_RELEASE) greedy_stt.cpp parse_input.o -DVARIANT=$* -o $@

parse_input.o: parse_input.h parse_input.cpp
	$(CC_RELEASE) -c parse_input.cpp


bin/greedy_stt_debug: greedy_stt.cpp parse_input_debug.o
	g++ -Wall -g -pedantic -std=c++20 greedy_stt.cpp parse_input_debug.o -o bin/greedy_stt_debug

parse_input_debug.o: parse_input.h parse_input.cpp
	g++ -c -Wall -g -pedantic -std=c++20 parse_input.cpp -o parse_input_debug.o

clean:
	rm -f *.o
	rm -rf bin
