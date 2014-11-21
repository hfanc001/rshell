#include <iostream>
#include <cmath>
#include <errno.h>
#include <fcntl.h>
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
/*
void fork_execvp(int cmds, char** cmd, bool next_pipe, string inputfile, string outputfile)
{
	int pid = fork();
	bool in = false, out = false;

	//-------------this is a test to see what----------- 
	//                       cmd has
	cout << "This is cmd: ";
	for(int i = 0; i < cmds; i++)
	{
		cout << cmd[i] << " ";
	}
	cout << endl;
	//------------------------------------------------
	

	//deal with input/output redirect
	//if there is a specified input, read from that file
	int fdin, fdout;
	if(inputfile != "")
	{
		in = true;
		fdin = open(inputfile.c_str(), O_RDONLY);
		if(fdin == -1)
		{
			perror("Error in open(input_file, O_RDONLY)");
			exit(1);
		}
		
		if(dup(0) == -1)
		{
			perror("Error in dup(0)");
			exit(1);
		}			
	}

	//if there is a specified output, output to there
	if(outputfile != "")
	{
		out = true;
		fdout = open(outputfile.c_str(), O_WRONLY);
		if(fdout == -1)
		{
			perror("Error in open(output_file, O_WRONLY)");
			exit(1);
		}
		
		if(close(1) == -1)
		{	
			perror("Error in close(1)");
			exit(1);
		}
		
		if(dup(1) == -1)
		{
			perror("Error in dup(1)");
			exit(1);
		}			
	}
	
	if(pid == -1) //the error check
	{
		perror("An error occured in fork()");
		exit(1); //exit because found error in fork
	}

	else if(pid == 0) //if fork returns 0 == in child process
	{
		//there is a specified output file)
		if(out)
		{
			if(close(1) == -1)
			{
				perror("Error in close(0)");
				exit(1);
			}

			int temp_out = dup(1);
			if(temp_out == -1)
			{
				perror("Error in dup(1)");
				exit(1);
			}
			
		}
	
		//no specified output file
		//check if there is more pipe to come		
		//make output stdin so the content can be read by next command
		else if(next_pipe)
		{

			if(-1 == close(0))
			{
				perror("Error in close(0)");
				exit(1);
			}				
			
			int temp_out = dup(0);
			if(temp_out == -1)
			{
				perror("Error in dup(0)");
				exit(1);
			}
		}

		
		if(-1 == execvp(cmd[0],cmd))
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

	return;
}
*/
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
						idirect = true;
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

					one_out = true;
					if(j != 0 && input.at(i).at(j-1) == '>')
					{
						one_out = false;
						two_out = true;
					}
					
				}						
			}
			
		}

		/*---test case to see what the user had input
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

		//-------end part of test case-----------------*/

		//convert input vector to a single modified string 
		int num_exe = 0;
		bool next_pipe = false;
		int it_ign = 0;
		int it_pip = 0;
		int it_cmd = 0;
		int in_size = input.size();
		int cmd_size = in_size + 1;
		char **cmd = new char* [cmd_size];
		int ig_size = ignore_list.size();
		int pi_size = pipe_location.size();

		int real_out = dup(1);
		if(real_out == -1)
		{
			perror("Error in real_out = dup(1)");
			exit(1);
		}

		int fd[2];
		if(pipe(fd) == -1)
		{
			perror("Error in pipe(fd)");
			exit(1);
				}
		
	
		for(int i = 0; i < in_size; i++)
		{
			if((it_ign < ig_size) && (i == ignore_list.at(it_ign)))
			{
				it_ign++;
				//cout << "__";
			}
	
			else if((it_pip < pi_size) && (i == pipe_location.at(it_pip)))
			{
				it_pip++;
				cmd[it_cmd] = NULL;
				//cout << it_cmd << " " << flush;
				
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
					fdin = open(input_file.c_str(), O_RDONLY);
					if(fdin == -1)
					{
						perror("Error in open(input_file, O_RDONLY)");
						exit(1);
					}

					if(dup2(fdin, 0) == -1)
					{
						perror("Error in dup2(fdin, 0)[333]");
						exit(1);
					}	

					//make the fdin the read end of the pipe
		
				}

		
				//read  from imaginary input
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
					num_exe++;
/*					if(close(1) == -1)
					{
						perror("Error in close(1)");
						exit(1);
					}
			
					int temp_out = dup(1);
					if(temp_out == -1)
					{
						perror("Error in dup(1)");
						exit(1);
					}
*/

					if(dup2(fd[1], 1) == -1)
					{
						perror("Error in dup2(fd[1], 1)");
						exit(1);
					}

					//meaning the input for cat is a string
					if(extra1)
					{
						//output my content to stdout which is stdin at this point
						cout << content << flush;
	
						//make if read from the pipe
						if(dup2(fd[0], 0) == -1)
						{
							perror("Error in dup2(fd[0], 0)");
							exit(1);
						}
					}
					
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
				char* temp = new char;
				strcpy(temp, input.at(i).c_str());
				cmd[it_cmd] = temp;
				it_cmd++;
				//cout << it_cmd << " " << flush;
			
				if(i == (in_size - 1))
				{
					cmd[it_cmd] = NULL;
					next_pipe = false; 

					//deal with input/output redirect
					//if there is a specified output, output to there
					int fdout;
					
					if(output_file != "")
					{
		
						if(one_out)
						{
							fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
							if(fdout == -1)
							{
								perror("Error in open(output_file, O_WRONLY | O_CREAT | O_TRUNC)");
								exit(1);
							}
						}

						else if(two_out)
						{
							fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND);
							if(fdout == -1)
							{
								perror("Error in open(output_file, O_WRONLY | O_CREAT | O_APPEND)");								    

							 exit(1);
							}
						}

						if(dup2(fdout, 1) == -1)
						{
							perror("Error in dup2(fdout, 1)");
							exit(1);
						}			
					}

	
/*					else
					{
						if(dup2(real_out, fd[1]) == -1)
						{
							perror("Error in dup2(real_out, fd[1]");
							exit(1);
						}
					}	
*/	
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
		
		for(int a = 0; a < pi_size; a++)
		{
			if(wait(0) == -1)
			{
				perror("Error in wait(0)");
				exit(1);
			}
		}

		delete cmd;

		/*-----------more test case----------
		cout << "the num_exe is " << num_exe << endl;
		cout << "The cmd size is: " << cmd_size << endl;
		cout << "The it_cmd is: " << it_cmd << endl;
		cout << "The it_ign is: " << it_ign << endl;
		cout << "The it_pip is: " << it_pip << endl;
		//-----------end more test case----------------*/

	}//the closing bracket for if on the very top

	return 0;
}
