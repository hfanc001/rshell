#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <vector>
#include <sstream>
using namespace std;

//this is a function for dashL to convert month from number to word
string toMonth(int tm_mon)
{
	if(tm_mon == 0)
		return "Jan";
	if(tm_mon == 1)
		return "Feb";
	if(tm_mon == 2)			
		return "Mar";
	if(tm_mon == 3)
		return "Apr";
	if(tm_mon == 4)	
		return "May";
	if(tm_mon == 5)
		return "Jun";
	if(tm_mon == 6)
		return "Jul";
	if(tm_mon == 7)
		return "Aug";
	if(tm_mon == 8)
		return "Sep";
	if(tm_mon == 9)
		return "Oct";
	if(tm_mon == 10)
		return "Nov";
	if(tm_mon == 11)	
		return "Dec";
	
	return 0;
}

//this function prints out fileName in color
//if it is executable, it prints the file out in green with a star in the end
//if it is directory, it prints the file out in blue with a slash in the end
//if it is a hidden file, it prints the file out with gray background
//all the colors are combinable
void print_col(const char *fileName, string path)
{
	//default: assume the file is not exectuable, a directory, nor a hidden file
	bool exec = false;
	bool direc = false;
	bool hidden = false;

	string temp = fileName;
		
	//syscall stat is to check link fileName
	//here use a struct buf to save the information
	struct stat buf;
	malloc(sizeof(struct stat));
	int status = stat(path.c_str(), &buf);	
	if(status == -1)
	{
		 perror("Error in stat(currFile.c_str(), &buf)[67]");
	}	
	
	//if the file has more than one link, it means it is a directory
	if(buf.st_nlink > 1)
	{
		//directories
		direc = true;
	}
	
	//use st_mode to check if the file is exectuable
	if(buf.st_mode & S_IXUSR)
	{
		//executable
		exec = true;
	}
	
	//the file is hidden if it starts with a dot
	if(fileName[0] == '.')
	{
		//ignore . and ..
		if(temp != "." && temp != "..")
		{
			//hidden
			hidden = true;
		}
	}

	//if the file is a dictory
	if(direc)
	{
		if(!hidden) //direct but not hidden -> blue
			cout << "\033[1;34m" << fileName << "/\033[0m ";
		
		else //direct but hidden -> blue and gray background
			cout << "\033[1;34m\033[1;40m" << fileName << "/\033[0m\033[0m ";
	}

	else if(exec)
	{
		//exec -> green
		cout << "\033[1;32m" << fileName << "*\033[0m ";
	}
			
	else
		cout << fileName << " ";

	return;			
}

//void dashL(const char *fileName, vector<const char*> direct)
void dashL(const char *fileName, string path)
{ 	
	//in case of -R path problem for stat
	//pass in vector of const char with directory info 
	//create a string called "currFile" with a completed path
	//use currFile instead of fileName
	string currFile;

	currFile += path;
	currFile += "/";
	currFile += fileName;
		
	//syscall stat is to check link and size and fileName
	//here use a struct buf to save the information
	struct stat buf;
	malloc(sizeof(struct stat));
	int status = stat(currFile.c_str(), &buf);	
	if(status == -1)
	{
		 perror("Error in stat(currFile.c_str(), &buf)[137]");
	}
	
	//syscall getpwuid is to get username 
	//from uid provided by syscall stat
	//here use a pointer to struct usrpwd
	struct passwd *usrpwd = getpwuid(buf.st_uid);
	if(usrpwd == NULL)
	{
		perror("Error in getwuid(buf.st_uid)[146]");
		exit(1);
	}
	
	//syscall getgrgid is to get groupname
	//from gid provided by syscall stat
	//here use a pointer to struct gppwd
	struct group *gppwd = getgrgid(buf.st_gid);
	if(gppwd == NULL)
	{
		perror("Error in getgrgid(buf.st_gid)[156]");
		exit(1);
	}
	
	//syscall localtime is to get last modified date and time
	//from th`e mtime provided by stat
	//here use a pointer to struct tm
	time_t t = buf.st_ctime;
	struct tm *tm;
	tm = localtime(&t);
	if(tm == NULL)
	{
		perror("Error in localtime(buf.st_ctime)[168]");
		exit(1);
	}	

	//use st_mode from stat to determine protection
	//USR means the permission to user
	//GRP means the permission to group
	//OTH means the permission to others
	
	if(S_ISDIR(buf.st_mode))
		cout << "d";
	else 
		cout << "-";
	if(buf.st_mode & S_IRUSR)
		cout << "r";
	else
		cout << "-";
	if(buf.st_mode & S_IWUSR)
		cout << "w";
	else
		cout << "-";
	if(buf.st_mode & S_IXUSR)
		cout << "x";
	else
		cout << "-";
	if(buf.st_mode & S_IRGRP)
		cout << "r";
	else
		cout << "-";
	if(buf.st_mode & S_IWGRP)
		cout << "w";
	else
		cout << "-";
	if(buf.st_mode & S_IXGRP)
		cout << "x";
	else
		cout << "-";
	if(buf.st_mode & S_IROTH)
		cout << "r";
	else
		cout << "-";
	if(buf.st_mode & S_IWOTH)
		cout << "w";
	else
		cout << "-";
	if(buf.st_mode & S_IXOTH)
		cout << "x";
	else
		cout << "-";

	//output the number of links
	cout << " " << buf.st_nlink << " ";
	//output the username
	cout << usrpwd->pw_name << " ";
	//output the groupname
	cout << gppwd->gr_name << " ";
	//output the file size
	cout << buf.st_size << " ";
	//output month and day
	cout << toMonth(tm->tm_mon) << " " << tm->tm_mday;
	//output time
	cout << " " << tm->tm_hour << ":" << tm->tm_min << " ";
	//output filename in color
	print_col(fileName, currFile.c_str());
	cout << endl;
	return;
}	

//this function is for -R tag
//it recursively goes into each directory and output all the files inside
void dashR(const char *dirName, string path, bool flag_a, bool flag_l)
{	
	//define a vector of const char to store all the files
	//that is a directory
	vector<const char*> repeat;

	//output the directory name 
	cout << path << ":" << endl;

	//open the directory 
    	DIR *dirp = opendir(path.c_str());
	if(dirp == NULL)
    	{	
		perror("Error in opendir(dirName)[251]");
		exit(1);
    	}
	

	//declare a struct of dirent
	dirent *direntp;
	//declare a struct of stat
    	malloc(sizeof(struct stat));
	
	//while the dirp directory can be read
    	while ((direntp = readdir(dirp)))
    	{ 
		if(direntp == NULL)
		{
			perror("Error in readdir(dirp)[267]");
			exit(1);
		}
	
		//define a const char and set as fileName
		const char *fileName = direntp->d_name;
		string currFile;
		
		//update currFile's path
		currFile += path;
		currFile += "/";
		currFile += fileName;

		//declare a struct 
		struct stat buf;
		malloc(sizeof(struct stat));
	
		int status = stat(currFile.c_str(), &buf);	
		if(status == -1)
		{
			perror("Error in stat(fileName, &buf)[287]");
			exit(1);
		}
	
		//if it is not -a, meaning dots can be ignored
		if(!flag_a)
		{
			//while dots are ignored
			if(fileName[0] != '.')
			{
				//if -l is entered
				if(flag_l)
					dashL(fileName, path);
				else
				{
					//if no -l, just output it
					print_col(fileName, currFile.c_str());
					cout << " ";
				}
				
				//it the file is a diirectory
				if(buf.st_nlink > 1)
				{
					//push the file into repeat if the file is a directory
					string temp = fileName;
					if(temp != "." && temp != "..")
					{
						repeat.push_back(direntp->d_name);
					}
				}		
		
			}
		}

		//if dots cannot be ignored
		else
		{
			//if -l is entered
			if(flag_l)
				dashL(fileName, path);
			else
			{
				//if no -l, just output it
				print_col(fileName, currFile.c_str());
				cout << " ";
			}

			//if the file is a directory
			if(buf.st_nlink > 1)
			{
				//push the file into repeat if the file is a directory
				string temp = fileName;
				if(temp != "." && temp != "..")
				{
					char* tempF;
					tempF = (char*) malloc (1024);
					strcpy(tempF, direntp->d_name);
					repeat.push_back(tempF);
				}
			}		
			

		}
	}

	//close the directory
	if(closedir(dirp) == -1)
    	{ 
		perror("Error in closedir(dirp)[352]");
		exit(1);
    	} 
	
	
	cout << endl;

	//define a vector of string to prevent pointer linking problem
	vector<string> repeat1;

	int r_size = repeat.size();
	for(int k = 0; k < r_size; k++)
	{
		repeat1.push_back(repeat.at(k));
	}

	//if there is any directory in the file
	if(repeat1.size() > 0)
	{
		//store the old path
		string newpath = path;
	
		//go into the repeated file vector to individually check each functions
		for(int i = 0; i < r_size; i++)
		{
			//if the directory name is blank, just ignore
			string compare = repeat1.at(i);
			if(compare == "")
				return;

			//update the path with the directory that is needed
			path += "/";
			path += repeat1.at(i);

			//call the function -R again
			dashR(repeat1.at(i).c_str(), path, flag_a, flag_l);
			//return to my old path
			path = newpath;
		}
	}
		

	return;

}
	
int main(int argc, char** argv)
{
    //declear a vector to read in commands
    //the first is always fonna be ls
    //check the rest if they are -a, -l, or -R
    //check if any specified directory is passed in 	
    vector<string> input;
     
    //string str;

    /*read in userinput
    do
    {
	input.clear();
     	getline(cin, str);
	istringstream iss(str);
	while(iss >> str)
    	{
		input.push_back(str);
    	}	 
	
    //if the user does not input anything with ls, ask the user to redo it
    }while(input.at(0) != "ls");*/

	for(int i = 0; i < argc; i++)
		input.push_back(argv[i]);

    bool flag_a = false;
    bool flag_l = false;
    bool flag_r = false;
    bool mem_address = false;
    string mem_add;

    if(input.size() > 1)
    {
		int i_size = input.size();
		for(int i = 1; i < i_size; i++)
		{
			if(input.at(i).at(0) == '-')
			{
				int temp_size = input.at(i).size();
				for(int j = 1; j < temp_size; j++)
				{
					if(input.at(i).at(j) == 'a')
						flag_a = true;
					else if(input.at(i).at(j) == 'l')
						flag_l = true;
					else if(input.at(i).at(j) == 'R')
						flag_r = true;
				}
			}
	
			else
			{
				mem_address = true;
				mem_add = input.at(i);
			}
		}
    }	
		
	//if(input.size() == 0)
	//	input.push_back("ls");
  
	char* DirName;
	DirName = (char*) malloc (1024);

    if(mem_address)
    {
		strcpy(DirName, mem_add.c_str());
    }

	else
	{
		string Dot = ".";
		strcpy(DirName, Dot.c_str());
	}

    const char *dirName = DirName;


    //if -R is passed in
    //check link size
    //if link is larger than 1
    //redo
    if(flag_r)
    {	
		//vector<const char*> direct;
		//direct.push_back(dirName);
		string path = dirName;
		//dashR(dirName, direct, flag_a, flag_l);
		dashR(dirName, path, flag_a, flag_l);
    }

    else
    {
		DIR *dirp = opendir(dirName);
		if(opendir(dirName) == NULL)
    	{	
			perror("Error in opendir(dirName)[376]");
			exit(1);
    	}

    	dirent *direntp;
    	//struct stat *buf;
    	//malloc(sizeof(struct stat));
    	while ((direntp = readdir(dirp)))
    	{ 
			if(direntp == NULL)
			{
				perror("Error in readdir(dirp)[387]");
				exit(1);
			}
	
			const char *fileName = direntp->d_name;
	
			//if -a is passed in
			//just print out fileName with newline	
			//if no -a, ignore all the dots
	

			if(flag_a && !flag_l && !flag_r)
			{
			
				print_col(fileName, dirName);
				cout << " ";
			}
	
			//if no flag is passed in
			//ignore the dots and print out filename
			if(!flag_a && !flag_l && !flag_r)
			{
				if(fileName[0] != '.')
				{
					print_col(fileName, dirName);
					cout << " ";
				}
			}

			//if -l is passed in
			//call the function dashL to output the detailed files info
			//dashL(fileName);	
			if(flag_l)
			{
				//vector<const char*> direct;
				//direct.push_back(dirName);
				string path = dirName;

				if(!flag_a)
				{
					if(fileName[0] != '.')
					{
						//dashL(fileName, direct);
						dashL(fileName, path);
					}
				}
	
				else
				{
					//dashL(fileName, direct);
					dashL(fileName, path);
				}
			}
	
	
		}
 
		if(closedir(dirp) == -1)
		{	 
			perror("Error in closedir(dirp)[432]");
			exit(1);
		}   
 

	}	
	return 0;
}

