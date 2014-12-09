#include <iostream>
#include <cmath>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
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

//function that deal with pipes and input/output redirection	
//NEED TO BE FIXED
/*int piping(vector<string> input, vector<string> pathV)
{
	
	//iterate through the loop and look for >, <, and |
		bool extra1 = false;
		bool one_out = false;
		bool two_out = false;
		string content = "";
		string input_file = "";
		string output_file = "";
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
						ignore_list.push_back(i);
					}
		
					if(j == 2 && (input.at(i).at(0) == '<') && (input.at(i).at(j) == '<'))
					{
						content = input.at(i+1);
						extra1 = true;
						input_file = "";
						ignore_list.push_back(i+1);
					}
				}

				else if(temp == '|') //detect a pipping 
				{
					if(j == 0 && j != (input.at(i).size() -1))
					{
						pipe_location.push_back(i);
					}
	
					else
					{
						pipe_location.push_back(i);
					}
				}
				
				else if(temp == '>')
				{
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

					one_out = true;
					if(j != 0 && input.at(i).at(j-1) == '>')
					{
						one_out = false;
						two_out = true;
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

		//-------end part of test case-----------------

		//convert input vector to a single modified string 
		int it_ign = 0;
		int it_pip = 0;
		int it_cmd = 0;
		int in_size = input.size();
		int cmd_size = in_size + 1;
		char **cmd = new char* [cmd_size];
		int ig_size = ignore_list.size();
		int pi_size = pipe_location.size();

		//save the original stdin for later use
		int restorestdin = dup(0);
		if(restorestdin == -1)
		{
			perror("Error in restorestdin");
			exit(1);
		}	

		int restorestdout = dup(1);
		if(restorestdout == -1)
		{
			perror("Error in restroestdout");
			exit(1);
		}

		//create a pipe with read end and write end to allow the commands between pipes communicate
		int fd[2];
		if(pipe(fd) == -1)
		{
			perror("Error in pipe(fd)");
			exit(1);
		}
		
		//a loop that iterate through the vector of commands
		for(int i = 0; i < in_size; i++)
		{

			//if reach file redirector symbol or pipes, ignore
			if((it_ign < ig_size) && (i == ignore_list.at(it_ign)))
			{
				it_ign++;
			}
	
			//if reach a pipe
			else if((it_pip < pi_size) && (i == pipe_location.at(it_pip)))
			{
				it_pip++;
				cmd[it_cmd] = NULL;
				
				int pid = fork();
				if(pid == -1)
				{
					perror("Error in pid = fork()");
					exit(1);
				}

				//deal with input/output redirect
				//if there is a specified input, read from that file
				int fdin;
				if(input_file != "" && it_pip == 1)
				{
					//open the specified input file
					fdin = open(input_file.c_str(), O_RDONLY);
					if(fdin == -1)
					{
						perror("Error in open(input_file, O_RDONLY)");
						exit(1);
					}
					
					//make the input file directory replace stdin
					if(dup2(fdin, 0) == -1)
					{
						perror("Error in dup2(fdin, 0)[333]");
						exit(1);
					}	

				}

		
				//read  from imaginary input if it is not the first command before pipe
				else if(it_pip != 1)
				{
					if(dup2(fd[0], 0) == -1)	
					{
						perror("Error in dup2(fd[0], 0)[352]");
						exit(1);
					}
				}

				else if(pid == 0) //if fork returns 0 == in child process
				{

					//output to imaginary pipe write end
					if(dup2(fd[1], 1) == -1)
					{
						perror("Error in dup2(fd[1], 1)");
						exit(1);
					}

					//meaning the input for cat is a string
					if(extra1 && (it_pip == 1))
					{
						//output my content to stdout which is pipe write end  at this point
						cout << content << flush;
	
						//make if read from the pipe read end
						if(dup2(fd[0], 0) == -1)
						{
							perror("Error in dup2(fd[0], 0)");
							exit(1);
						}
						
						if(close(fd[1]) == -1)
						{
							perror("Error in close(fd[1])");
							exit(1);
						}
					
					}
					
					//call execvp to exectue the command
					if(-1 == execvp(cmd[0],cmd))
					{
						perror("An error occured in execvp");
						exit(1);
					}
	
					exit(1);//kill the child after its done
				}
		
				it_cmd = 0;
				//cout << it_cmd << " " << flush;
			}

			
			else
			{
				//if regular middle of a command, just add to char array
				char* temp = new char;
				strcpy(temp, input.at(i).c_str());
				cmd[it_cmd] = temp;
				it_cmd++;
			
				//if read end of the whole command line
				if(i == (in_size - 1))
				{
					cmd[it_cmd] = NULL;

					//deal with input/output redirect
					//if there is a specified output, output to there
					int fdout;
					
					if(output_file != "")
					{
						//if it is >, meaning the file will be over-written		
						if(one_out)
						{
							fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
							if(fdout == -1)
							{
								perror("Error in open(output_file, O_WRONLY | O_CREAT | O_TRUNC)");
								exit(1);
							}
						}

						//if it is >>, meaning the file will be appended
						else
						{
							fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
							if(fdout == -1)
							{
								perror("Error in open(output_file, O_WRONLY | O_CREAT | O_APPEND)");
								exit(1);
							}
						}

						//make output the file directory created for specified output
						if(dup2(fdout, 1) == -1)
						{
							perror("Error in dup2(fdout, 1)");
							exit(1);
						}			
					}

	
					int pid = fork();
					if(pid == -1) //the error check
					{
						perror("An error occured in fork()");
						exit(1); //exit because found error in fork
					}

					else if(pid == 0) //if fork returns 0 == in child process
					{
						//make it read from imaginary stdin
						if(dup2(fd[0], 0) == -1)
						{
							perror("Error in dup2(fd[0], 0)");
							exit(1);
						}
						
						if(close(fd[1]) == -1)
						{
							perror("Error in close(fd[1])");
							exit(1);
						}
										
						//call execvp to exectue the command
						if(-1 == execvp(cmd[0],cmd))
						{
							perror("An error occured in execvp");
							exit(1);
						}
		
						exit(1); //kill child after its done
					}	


				}
			}
		}		

		//restore the stdin back 
		if(dup2(restorestdin, 0) == -1)
		{	
			perror("Error in dup2(restorestdin, 0)");
			exit(1);
		}
		
		//restore the stdout back
		if(dup2(restorestdout, 1) == -1)
		{
			perror("Error in dup2(restorestdout, 1)");
			exit(1);
		}

		//if it was parent, it should be stopped here
		for(int a = 0; a < pi_size; a++)
		{
			if(wait(0) == -1)
			{
				perror("Error in wait(0)");
				exit(1);
			}
		}

		delete cmd;

		//-----------more test case----------
		cout << "The cmd size is: " << cmd_size << endl;
		cout << "The it_cmd is: " << it_cmd << endl;
		cout << "The it_ign is: " << it_ign << endl;
		cout << "The it_pip is: " << it_pip << endl;
		//-----------end more test case----------------
	return 0;
}*/

//declare connect function for connector
//take in input and path vector
void connect(vector<string> input, vector<string> pathV);

//function that execute commands 
//use EXECV and path instead of execvp
int execute(char **cmd, vector<string> pathV)
{
	//have the vectors with all the commands
	//have the vectors with all the possible paths
	int pSize = pathV.size();
	
	//add in / and command to make the path point to possible files
	for(int i = 0; i < pSize; i++)
	{
		pathV.at(i) += "/";
		pathV.at(i) += cmd[0];
	}

	//check the content of pathV
	//cout << "pathV: " << endl;
	//for(int i = 0; i < pSize; i++)
	//	cout << pathV.at(i) << endl;
	//cout << endl;

	//if there is an error, will return 1 later
	bool reOne = true;
	//for every possible path
	for(int i = 0; i < pSize; i++)
	{
		//check if the file is executable
		int acc = access(pathV.at(i).c_str(), X_OK);
		
		//perform error check on access
		if(acc > 0)
		{
			perror("Error in access in execute");
			exit(1);
		}

		//if the file is executable
		else if(acc == 0)
		{
			reOne = false;
			int pid = fork();
			
			//perform error check on fork
			if(pid == -1)
			{
				perror("Error in fork in execute");
				exit(1);
			}
			
			//if child process
			else if(pid == 0)
			{
				//run and error check execv
				if(execv(pathV.at(i).c_str(), cmd) == -1)
				{
					perror("Error in execv in execute");
					exit(1);		
				}

				//kill the child to prevent zombie process
				exit(1);
			}
			
			//if parent process
			else if(pid > 0)
			{
				//perform error check on wait
				if(wait(0) == -1)
				{
					perror("Error in wait in execute");
					exit(1);
				}
 			}
		}//closing braket for acc
	}//closing braket for for loop check

	if(reOne)
		return 1;
	else
		return 0;
}

//function that change the directory
void changedir(vector<string> input)
{
	//vector<string> newDir;
	int inputSize = input.size();
	string newDir;
	for(int i = 0; i < inputSize; i++)
	{
		if(input.at(i) == "cd")
		{
			if(i < (inputSize - 1))
				newDir = input.at(i+1);	
			//newDir.push_back(input.at(i+1));
		}
	}

	//run and error check for chdir	
	if(chdir(newDir.c_str()) == -1)
	{
		perror("Error in chdir(newDir)");
		exit(1);
	}

	/*if requires to read in multiple newDir for cd, use vector
	int newDirSize = newDir.size();
	cout << "newDir size: " << newDirSize << endl;
	for(int i = 0; i < newDirSize; i++)
	{
		cout << "path to change to: " << newDir.at(i) << endl;
		if(chdir(newDir.at(i).c_str()) == -1)
		{
			perror("Error in chdir(newDir)");
			exit(1);
		}
	}*/ 
}

//if ^C is entered
void ctrl_c(int signal)
{
	//output a newline to make the format clean
	cout << endl;
	return;
}

//if ^Z is entered
void ctrl_z(int signal)
{
	//pause the foreground
	if(raise(SIGSTOP) != 0)
	{
		perror("Error in raise(SIGSTOP)");
		exit(1);
	}
}

int main(int argc, char ** argv)
{
	/*assignment 1 extra credit: DISPLAY USERNAME AND HOSTNAME
	//first set the size of the username and the hostname
	//and then set it as 100 so there could be a lot of space for it
	char uname[2048];
	string username;
	char hname[1024];

	if(getlogin_r(uname, 2048) != 0)
		perror("Error in getlogin_r");
	if(gethostname(hname, 1024) != 0)
		perror("Error in gethostname");*/
	
	//variables to find the path
	const char *symlinkpath = ".";
	char actualpath[2048];
	char *ptr;

	//declare the signals first
	//if ^C is entered
	signal(SIGINT, ctrl_c);
	if(false)
	{
		perror("Error in signal(SIGINT, ctrl_c)");
		exit(1);
	}

	//if ^z is entered
	signal(SIGTSTP, ctrl_z);
	if(false)
	{
		perror("Error in signal(SIGTSTP, ctrl_z)");
		exit(1);
	}
	
	//an infinite loop to always ask user input
	while(1)
	{
		//always print out $ and ask for input
		//need to get input
		string word = "";
		vector<string> input;

		//take input from the user
		while(word == "")
		{
			//assignment 1 extra credit: DISPLAY USERNAME AND HOSTNAME
			//cout << uname << "@" << hname << "$ ";

			//assignment 3 requirement: DISPLAY PATH
			ptr = realpath(symlinkpath, actualpath);
			if(ptr == NULL)
			{
				perror("Error in realpath in while(1)");
				exit(1);
			}
			cout << ptr << " $ ";

			//read in input with getline
			getline(cin, word);
		}
		
		//output the input to istringstream buffer and extract them into vectors
		istringstream iss(word);
		while(iss >> word)
			input.push_back(word);
			
		//if "exit" is entered, exit the program
		if(word == "exit")
			exit(0);

		//check if connectors, pipes, or cd
		bool connectors = false;
		bool pipes = false;
		bool cd = false;
		int input_size = input.size();
		
		//declare variable to check for comments
		bool comment = false;
		int loc_i = 0;
		int loc_j = 0;

		//loop through input to check for connectors or pipes
		for(int i = 0; i < input_size; i++)
		{
			int inSize = input.at(i).size();
			for(int j = 0; j < inSize; j++)
			{
				char temp = input.at(i).at(j);
				if(temp == '&' || temp == ';')
					connectors = true;
				
				else if(temp == '>' || temp == '<')
					pipes = true;
		
				else if(temp == '|')
				{
					if(j > 0)
					{	
						if(input.at(i).at(j - 1) == '|')
						{
							connectors = true;
							pipes = false;
						}

						else
							pipes = true;
					}
				}
				
				//check for comment and save the locations (i, j)
				else if(temp == '#')
				{
					comment = true;
					loc_i = i;
					loc_j = j;
				}
			}
	
			if(input.at(i) == "cd")
				cd = true;
		}
		
		/*cout << "input" << endl;
		for(int i = 0; i < input_size; i++)
			cout << input.at(i) << " ";
		cout << endl;*/

		//check if there is a comment, if there is, pop back all the things after that
		if(comment)
		{
			int comI_size = input.size();
			comI_size -= loc_i;
			comI_size--;

			for(int i = 0; i < comI_size; i++)
				input.pop_back();
			
			int last = input.size();
			last--;
			
			int comJ_size = input.at(last).size();
			comJ_size -= loc_j;

			//cout << "(" << loc_i << "," << loc_j << ")" << endl;
			string com_s = "";
			for(int j = 0; j < loc_j; j++)
				com_s += input.at(last).at(j);
			input.at(last) = com_s;
			//cout << input.at(last) << endl;
		}

		/*cout << "input" << endl;
		for(int i = 0; i < input_size; i++)
			cout << input.at(i) << " ";
		cout << endl;*/

		//AFTER ONE ITERATION, THE PATH IS DIFFERENT AND WEIRD
		// because the original PATH was changed(security issue)
		//after getting path from getenv, need to copy the content to other pointer
		//then change that other pointer so the original content can be saved
		char* path = getenv("PATH");
		char* pPath;
		pPath = (char*) malloc (1024);
		strcpy(pPath,path);
		if(pPath == NULL)
		{
			perror("Error in getenv");
			exit(1);
		}
		//cout << "Path" << endl << pPath << endl; //output path to check
		
		//parse the path
		char *path1[2048];
		vector<string> pathV;
		int temp_i = 0;
		pPath = strtok(pPath, ":");
		if(pPath == NULL)
		{
			perror("End of path in strtok");
			exit(1);
		}
		
		while(pPath != NULL)
		{
			//output path to check the individual path
			//cout << path << endl; 
			path1[temp_i] = pPath;
			if(path1[temp_i] != NULL)
				pathV.push_back(path1[temp_i]);
			pPath = strtok(NULL, ":");

			//need to do error check but this has no erro
			//if return NULL, the function will exist itself
			if(pPath == NULL)
			{
				if(pPath != NULL)
					perror("Error in path");
			}
		}
					
		//if there is a pipe or input/output redirections, call the function		
		if(pipes);
			//piping(input, pathV);
	
		//if there is a connector, call the function 
		else if(connectors)
			connect(input, pathV);

		//if there is cd, call the function to change directory
		else if(cd)
			changedir(input);	

		//if nothing special, just call regular execute function
		else
		{
			//declare variable
			int inputSize = input.size();
			char ** cmd = new char*[inputSize + 1];

			//parse the command first
			for(int i = 0; i < inputSize; i++)
			{
				char* temp = new char;
				strcpy(temp, input.at(i).c_str());
				cmd[i] = temp;
			}
			cmd[inputSize] = NULL;
	
			//call the execute function to run the command
			execute(cmd, pathV);
		}

	}//the closing bracket for if on the very top

	return 0;
}

//declare connect function for connector
//take in input and path vector
void connect(vector<string> input, vector<string> pathV)
{
		int inputSize = input.size();

		//check the content of input
		/*cout << "input" << endl;
		for(int i = 0; i < inputSize; i++)
			cout << input.at(i) << endl;*/

		//check the content of path
		/*cout << "path" << endl;
		for(int i = 0; i < pathSize; i++)
			cout << pathV.at(i) << endl;*/
			
		//declare variable
		char ** cmd = new char*[inputSize + 1];
	
		bool front = false;
		string front_t;
		int count = 0;
		int con = 0;

		//parse the command first
		for(int i = 0; i < inputSize; i++)
		{
			string temp = "";
			int inSize = input.at(i).size();
			
			//if in the last iteration, there was a connector in the front
			if(front)
			{
				//reset front bool and store the command read
				front = false;
				temp = front_t;
			}

			else
			{
				//iterate through each char in the string to check connector
				for(int j = 0; j < inSize; j++)	
				{
					char temp1 = input.at(i).at(j);
					//cout << temp1 << " " << flush;
					
					//if it is not a connector, push the char to the temp string
					if(temp1 != '&' && temp1 != '|' && temp1 != ';')
						temp += temp1;

					//if && connector is found
					//set con to one and check if it is in the front
					else if(temp1 == '&')
					{
						con = 1;
						if(j == 0 && (inSize > 2))
							front = true;
					}
				
					//if || connector is found
					//set con to two and check if it is in the front
					else if(temp1 == '|')
					{
						con = 2;
						if(j == 0 && (inSize > 2))
							front = true;
					}

					//if ; connector is found
					//set con to three and check if it is in the front
					else if(temp1 == ';')
					{
						con = 3;
						if(j == 0 && (inSize > 2))
							front = true;
					}

				}
			}

			//if the connector is not in the front or is a regular command
			//push the temp stirng to the command array
			if(!front)
			{
				char* temp2 = new char;
				strcpy(temp2, temp.c_str());
				cmd[count] = temp2;
				count++;
			}

			//if there is a connector
			if(con != 0)
			{
				//put an end to the command array
				cmd[count] = NULL;
				//call execute function 
				int exe = execute(cmd, pathV);
				//if an error is returned 
				if(exe != 0)
				{
					//output error message
					cout << "Command not found" << endl;
					//if the connector is ||, return when the first command fails
					if(con == 2)
						return;
				}
				//make the command array iterator zero
				count = 0;
			}	

			//if the connector appear in the front of the word
			//make the loop go back one to iterate again
			//but with the already got information
			if(front)
			{
				front_t = temp;
				i--;
			}

		}

}
