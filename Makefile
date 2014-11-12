all:	bin rshell cp ls

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	g++ src/rshell.cpp -o ./bin/rshell

cp: 	
	g++ -Wall -Werror -ansi -pendatic src/cp.cpp -o ./bin/cp
ls:
	g++ -Wall -Werror -ansi -pendatic src/ls.cpp -o ./bin/ls
