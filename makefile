all: rshell
	rm -rf bin
	mkdir bin
	g++ -std=c++11 -Wall -Werror -pedantic src/main.cpp -o bin/rshell
rshell:
	rm -rf bin
	mkdir bin
	g++ -std=c++11 -Wall -Werror -pedantic src/main.cpp -o bin/rshell

