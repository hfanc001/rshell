all:	bin rshell cp ls 

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	g++ -Wall -Werror -ansi -pedantic src/rshell.cpp -o ./bin/rshell
ls:
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o ./bin/ls
cp:
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o ./bin/cp
