FRQ:
----

####`Rshell`:
1. Shell introduction: [Writing Your Own Shell](http://linuxgazette.net/111/ramankutty.html)

####`LS`:
ls -a 
	get the current directory 
	open the current directory 
	output the files
	close the working directory

ls
	same as ls -a
	but do not output the hidden file

ls -l
	get the working directory
	open the working directory
	check the read/write permission
	print the necessary informaiton (read/write permission, username, date)
	print the working directory

	Output order:
		1. file permission (stat, st_mode)
		2. number of links (stat, st_nlink)
		3. owner (stat, st_uid) (use `getpwuid` to find the particular UID)
		4. group (stat, st_gid) (use `getgrgid` to find the name with GID)
		5. size (stat, st_size) 
		6. last modified date & time (stat, st_mtime) (use `localtime`)
		7. file name

	*use open to get the read/write permission
	*`printf` can be used to format [AWK Language: Printing Output](http://www.chemie.fu-berlin.de/chemnet/use/info/gawk/gawk_7.html)

ls -R 
	get the current directory 
	open the current directory
	print out the files/save any directory inside this working directory
	use a loop or recursive call to go to each directory
	print out the files inside that directory
	close the working directory

	*if the output is weird ASCII character, try saving repeated filename in stack instead of heap

#####FRQ: 
1. [Howto: C Programming with Directories on Linux](http://www.thegeekstuff.com/2012/06/c-directory/)
2. [Stackoverflow: How to implement Unix ls -l Command in C](http://stackoverflow.com/questions/857156/how-to-implement-unix-ls-l-command-in-c)
3. [Stackoverflow: Implementing ls command in C](http://stackoverflow.com/questions/14576725/implementing-ls-command-in-c) (output formatting source code)
4. [Unix LS Command: 15 Practical Examples](http://www.thegeekstuff.com/2009/07/linux-ls-command-examples/)
5. Check read/write permission, [`access` Man Page](http://linux.die.net/man/2/access)
6. Check file permission [Stackoverflow: Printing file permissions like "ls -l" using stat(2) in C] (http://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c)
7. Check path name:
	[Stackoverflow: How can I check whether this is directory-path or any filename-path] (http://stackoverflow.com/questions/8427436/how-can-i-check-whether-this-is-directory-path-or-any-filename-path)

