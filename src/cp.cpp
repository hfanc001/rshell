#include <iostream>
#include "Timer.h"
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main() 
{	
	ifstream in("in.text");
	ofstream out("out1.text");

	//THE FIRST METHOD
	//USING IN.GET(CHAR) AND OUT.PUT(CHAR) TO COPY THE INPUT AND OUTPUT FILE OEN AT A TIME
		
	char c;
	while(in.good())
	{		
		c = in.get();
		if(in.good())
		{
			out.put(c);		
			//cout << c << " ";
		}
	}

	//cout << "End of first method.\n";

	//THE SECOND METHOD
	//USING READ() AND WRITE() TO COPY THE INPUT AND OUTPUT FILE ONE AT A TIME
	
	char BUF[BUFSIZ];
	int num;
	int result;

	result = access("out2.text", F_OK);
	if(result == 0)
	{
		perror("An error occured in accessi_1");
		exit(1);
	}
	
	int fdi = open("in.text", O_RDONLY);
	if(fdi == -1)
	{
		perror("An error occured in opening fdi_1");
	}
	
	int fdo = open("out2.text", O_WRONLY |  O_CREAT);
	if(fdo == -1)
	{
		perror("An error occured in opening fdo_1");
	}

	while((num = read(fdi, BUF, 1)))
	{	
		if(num == -1)
		{
			perror("An error occured in read_1");
			exit(1);
		}

		if(write(fdo, BUF, 1) == -1)
		{
			perror("An error occured in write_1");
		}
	}
	
	//THE THIRD METHOD
	//USING READ() AND WRITE() TO COPY THE INPUT AND OUTPUT FILE A BUFFER AT A TIME
	
	//char BUF[BUFSIZ];
	//int num;
	//int result;
	result = access("out3.text", F_OK);
	if(result == 0){
	 
		perror("An error occured in access_2");
		exit(1);
	}
	
	int fdi2 = open("in.text", O_RDONLY);
	if (fdi2 < 0)
	{
		perror("An error occured in opening fdi_2");
	}
	
	int fdo2 = open("out3.text", O_WRONLY | O_CREAT);
	if(fdo == -1)
	{
		perror("An error occured in opening fdo_2");
	}

	int number;
	while((number = read(fdi2, BUF, BUFSIZ)))
	{	
		if(number == -1)
		{
			perror("An error occured in read_2");
			exit(1);
		}

		if(write(fdo2, BUF, BUFSIZ) == -1)
		{
			perror("An error occured in write_2");
		}
	}
	
	return 0;
	
}
