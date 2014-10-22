all: test

test:
	rshell_fun.o exec.o
	g++ rshell_fun.o exec.o -o test

exec.o:
	exec.cpp
	g++ -c exec.cpp

rshell_fun.o:
	rshell_fun.h rshell_fun.cpp
	g++ -c rshell_fun.h rshell_fun.cpp




