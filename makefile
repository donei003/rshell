all: rshell ls cp mv 

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
mv:
	mkdir bin -p
	g++ -std=c++11 -Wall -Werror -ansi -pedantic src/mv.cpp -o bin/mv
rm:
	mkdir bin -p
	g++ -std=c++11 -Wall -Werror -ansi -pedantic src/rm.cpp -o bin/rm
