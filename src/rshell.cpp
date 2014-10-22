#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "rshell_fun.h"
#include <vector>
#include <sstream>
using namespace std;

int main(int argc, char ** argv)
{
	//first set the size of the username and the hostname
	//and then set it as 100 so there could be a lot of space for it
	char username[100];
	char hostname1[100];
	
	//call getlogin_r and check if it works
	int login = getlogin_r(username, sizeof(username - 1));
	if(login != 0)
	{
		perror("An error occured in getlogin()");
	}

	//call gethostname and check if it works
	int hostname2 = gethostname(hostname1, sizeof(hostname1) - 1);
	
	if(hostname2 != 0)
	{
		perror("An error occured in gethostname");
	}
		
	//always print out $ and ask for input
	while(1)
	{
		cout << "New" << endl;
		//need to get input
		string word = "";
		int pos = 0;

		//take input from the user
		while(word == "")
		{
			cout << username << "@" << hostname1 << "$ ";
			getline(cin, word);
		}
	
		if(word == "exit")
		{
			exit(0);
		}

		//the bool that check if there is a connecter in the command entered
		bool connecter = false;
		//check and know what connecter type it is
		int connecter_type = 0;
		//so the program knows how many more commands to do
		//used for connecter
		int commandsToDo = 0;
	
		int wsize = word.size();
		//check if there is a connector 
		for(int i = 0; i < wsize; i++)
		{
			//check if there is any sort of connecter in the command
			if(word.at(i) == '|' || word.at(i) == '&' || word.at(i) == ';')
			{
				//if comes into this if
				//there is a connecter
				//set connecter to true
				connecter = true;
				
				//if the connecter is a pipe
				//set the connecter_type as 1
				if(word.at(i) == '|')
				{
					connecter_type = 1;
				}
				
				//if the connecter is an and
				//set the connecter_type as 2
				if(word.at(i) == '&')
				{
					connecter_type = 2;
				}

				//if the connecter is an semicolon
				//set the connecter_type as 3
				if(word.at(i) == ';')
				{
					connecter_type = 3;
				}
			}
		}

		//find a connecter 
		//if connecter_type is 1 -> found a pipe
		//if connecter_type is 2 -> found an and
		//if connecter_type is 3 -> found a semicolon
		//in the functions, add one to commandsToDo
		//each time when a connecter is found
		//if(connecter)
	
		//count how many words there are in order to allocate cmd
		for(int i = 0; i < wsize; i++)
		{
			if(word.at(i) == ' ')
			{
				pos++;
			}
		}	

		pos += 2;
		const int posi = pos;
		//allocate cmd for later execvp() use
		char *urg[posi];

		//output the userinput in stringstream in order
		//to seperates words in sentences into char array
		stringstream ss(word);		

		//seperating sentence into char array using 
		//temp string and dynamically allocate 
		//memory in stack
		for(int i = 0; i <= pos - 2; i++)
		{
			string temp;
			ss >> temp; 	
			int temp_size = temp.size();
			urg[i] = new char[temp_size];
			//urg[i] = temp;
			//urg[i] = (char *)alloca(temp.size()+1);
			memcpy(urg[i], temp.c_str(), temp.size() + 1);
		}
		cout << endl;
		
		pos--;
		//add NULL to the end of cmd to complete the array
		urg[pos] = NULL;
/*		
		//check if everything works
		for(int i = 0; i < pos; i++)
		{
			cout << "This is the string in cmd[";
			cout << i << "] " << cmd[i] << endl;
		}
*/

		//read in the command and seperates into groups
		//use fork first to determine child/paremt
		//if under child, run execvp
		//if under parent, wait
	
		//if the command is a regular one 
		//ex. $ls -a or $echo hello
		//directly send it into execvp
		
		//else if the command is one connected with ;
		//ex. $ ls -a; echo hello; mkdir test
		//write a function to seperate the command by ;
		//and use a loop to exectue each commands
	
		//else if the command is connected with ||
		//write a funciton to seperately exectute each commands
	
		//else if the command is connected with &&
		//wrtie a funciton to seperately the commands
		//only move on to the second one if the first one works
		
		do
		{
			int pid = fork();
	
			if(pid == -1) //the error check
			{
				perror("An error occured in fork()");
				exit(1); //exit because found error in fork
			}

			else if(pid == 0) //if fork returns 0 == in child process
			{
				execvp(urg[0], urg);
			
				if(-1 == execvp(urg[0], urg))
				{
					perror("An error occured in execvp");
					exit(1);
				}
			
				exit(1); //kill child after its done
			}

			else if(pid > 0) //if fork returns something else = in parent
			{
				if(-1 == wait(0)) //wait for child to finish
				{
					perror("An error occured in wait()");
					exit(0);
				}
			}
			
		}while(commandsToDo != 0);
	}//the closing bracket for if on the very top

	return 0;
}
