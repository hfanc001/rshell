#include <iostream>
#include <cmath>
#include <errno.h>
#include <string>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <pwd.h>
using namespace std;

int main(int argc, char ** argv)
{
	//first set the size of the username and the hostname
	//and then set it as 100 so there could be a lot of space for it
	char uname[2048];
	string username;
	char hname[1024];

	if(getlogin_r(uname, 2048) != 0)
		perror("Error in getlogin_r");
	if(gethostname(hname, 1024) != 0)
		perror("Error in gethostname");
	

	//always print out $ and ask for input
	while(1)
	{
		//need to get input
		string word = "";
		vector<string> input;
		int len = 1;

		//take input from the user
		while(word == "")
		{
			cout << uname << "@" << hname << "$ ";
			getline(cin, word);
		}
		
		istringstream iss(word);
		while(iss >> word)
		{
			input.push_back(word);
			len++;
		}
	
		if(word == "exit")
		{
			exit(0);
		}
		
		//iterate through the loop and look for >, <, and |
		bool idirect = false;
		bool odirect = false;
		bool pipe = false;

		string input_file;
		string output_file;
		vector<string> pipping_commands;
		vector<int> ignore_list;
		vector<int> pipe_location;

		for(unsigned int i = 0; i < input.size(); i++)
		{
			for(unsigned int j = 0; j < input.at(i).size(); j++)
			{
				char temp = input.at(i).at(j);
				if(temp == '<') //detect an input redirection
				{
					if(j == 0 && i < (input.size()-1))
					{
						input_file = input.at(i + 1);
						idirect = true;
						ignore_list.push_back(i);
						//ignore_list.push_back(i + 1);
					}
				}

				else if(temp == '|') //detect a pipping 
				{
					pipe = true;
					if(j == 0 && j != (input.at(i).size() -1))
					{
						pipe_location.push_back(i);
						//input.at(i).at(j) = '\'
						//input.at(i).at(j + 1) = '0';
					}
	
					else
					{
						pipe_location.push_back(i);
						//input.insert(i, "\0");
					}
				}
				
				else if(temp == '>')
				{
					odirect = true;
					//if only one output rediector
					//meaning we will be overwriting it
					if(j == 0 && ((j + 1) == input.at(i).size()))
					{
						if(i != (input.size() - 1))
						{
							output_file = input.at(i + 1);
							ignore_list.push_back(i);
							ignore_list.push_back(i + 1);
						}
					}
					
				}						
			}
		}
	
		//---test case to see what the user had input
		cout << "The inputs are: ";
		for(unsigned int i = 0; i < input.size(); i++)
			cout << input.at(i) << " ";
		cout << endl;
		
		cout << "The ignore list is: ";
		for(unsigned int i = 0; i < ignore_list.size(); i++)
			cout << ignore_list.at(i) << " ";
		cout << endl;

		cout << "The pipe location is: ";
		for(unsigned int i = 0; i < pipe_location.size(); i++)
			cout << pipe_location.at(i) << " ";
		cout << endl;

		cout << "Input file is: " << input_file << endl;
		cout << "Output file is: " << output_file << endl;
		
		cout << "The size of input is: " << input.size() << endl;
		cout << "The size of ignore list is: " << ignore_list.size() << endl;
		cout << "The size of pipe location is: " << pipe_location.size() << endl;

		//-------end part of test case-------------------------

		//convert input vector to a single modified string 
		string temp_arr;
		int it_ign = 0;
		int it_pip = 0;
		int in_size = input.size();

		for(int i = 0; i < in_size; i++)
		{
			int ig_size = ignore_list.size();
			int pi_size = pipe_location.size();
			if((it_ign < ig_size) && (i == ignore_list.at(it_ign)))
			{
				it_ign++;
			}
	
			else if((it_pip < pi_size) && (i == pipe_location.at(it_pip)))
			{
				it_pip++;
				string temp = "|";
				temp_arr += temp;
			}

			else
			{
				temp_arr += input.at(i);
			}
		}
		
		//-----------more test case----------
		cout << "The string is: " << temp_arr << endl;		
		cout << "The string size is: " << temp_arr.size() << endl;
		//-----------end more test case----------------

		//allocate cmd for later execvp() use
		char *cmd = new char [temp_arr.size() + 1];
		strcpy(cmd, temp_arr.c_str());

		int tarr_size = temp_arr.size();
		cout << "The carr array is: " << endl;
		for(int k = 0; k < tarr_size; k++)
			cout << cmd[k] << " ";
		cout << endl;
			
		cout << "Test: " << endl;
		for(int k = -0; k < tarr_size; k++)
		{
			if(cmd[k] == '|')
			{
				cmd[k] = '\0';
				cout << "_";
			}

			else
				cout << cmd[k] << " ";
		}
		cout << endl;

/*		
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
				//execvp(urg[0], urg);
			
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
*/	}//the closing bracket for if on the very top

	return 0;
}
