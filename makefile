all: rshell

rshell:
	mkdir bin -p
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
			
clean:
	rm -rf bin/

cp:
	mkdir bin -p
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
