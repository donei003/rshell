all: rshell ls

rshell:
	mkdir bin -p
	g++ -std=c++11 -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
			
clean:
	rm -rf bin/

cp:
	mkdir bin -p
	g++ -std=c++11 -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
ls:
	mkdir bin -p
	g++ -std=c++11 -Wall -Werror -ansi -pedantic src/ls.cpp -o bin/ls
