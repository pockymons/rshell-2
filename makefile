all: rshell ls
rshell:
	rm -rf bin
	mkdir bin
	g++ -std=c++11 -Wall -Werror -pedantic src/main.cpp -o bin/rshell
ls:
	g++ -g -std=c++11 -Wall -Werror -pedantic src/ls.cpp -o bin/ls
