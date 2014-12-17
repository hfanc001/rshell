####`Rshell`:
-------------
#####FAQ in `rshell`:
1. Shell introduction: [Writing Your Own Shell](http://linuxgazette.net/111/ramankutty.html)
2. Strtok introduction: [Fresh2refresh C Programming Tutorial: C- strtok() Function](http://fresh2refresh.com/c/c-strings/c-strtok-function/)


####`LS`:
---------
#####`ls -a`
	get the current directory 
	open the current directory 
	output the files
	close the working directory

#####`ls`
	same as ls -a
	but do not output the hidden file

#####`ls -l`
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

use [open](http://linux.die.net/man/2/open) to get the read/write permission
`printf` can be used to format [AWK Language: Printing Output](http://www.chemie.fu-berlin.de/chemnet/use/info/gawk/gawk_7.html)

#####`ls -R`
	get the current directory 
	open the current directory
	print out the files/save any directory inside this working directory
	use a loop or recursive call to go to each directory
	print out the files inside that directory
	close the working directory

If the output is weird ASCII character, try saving repeated filename in stack instead of heap

#####FAQ in ls: 
1. [Howto: C Programming with Directories on Linux](http://www.thegeekstuff.com/2012/06/c-directory/)
2. [Stackoverflow: How to implement Unix ls -l Command in C](http://stackoverflow.com/questions/857156/how-to-implement-unix-ls-l-command-in-c)
3. [Stackoverflow: Implementing ls command in C](http://stackoverflow.com/questions/14576725/implementing-ls-command-in-c) (output formatting source code)
4. [Unix LS Command: 15 Practical Examples](http://www.thegeekstuff.com/2009/07/linux-ls-command-examples/)
5. To check read/write permission, use [access()](http://linux.die.net/man/2/access)
6. Check file permission [Stackoverflow: Printing file permissions like "ls -l" using stat(2) in C] (http://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c)
7. Check path name:
	[Stackoverflow: How can I check whether this is directory-path or any filename-path] (http://stackoverflow.com/questions/8427436/how-can-i-check-whether-this-is-directory-path-or-any-filename-path)

###`Piping`:
------------
#####`dup`
Use [dup()](http://linux.die.net/man/2/dup) or [dup2()](http://linux.die.net/man/2/dup2) to open new input/output redirection with the initialized input/output test file

#####FAQ in dup:
1. Pipe Explanation [Stackoverflow: Can Someone Explain What dup() in C Does?](http://stackoverflow.com/questions/7861611/can-someone-explain-what-dup-in-c-does?rq=1)
2. [Advance Bash-Scripting Guide: Chapter 20. I/O Redirection](http://www.tldp.org/LDP/abs/html/io-redirection.html)
2. [Stackoverflow: Having trouble with fork(), pipe(), dup2(), and exec() in C](http://stackoverflow.com/questions/916900/having-trouble-with-fork-pipe-dup2-and-exec-in-c?rq=1)
3. [Stackoverflow: Implementation of multiple pipes in C](http://stackoverflow.com/questions/8389033/implementation-of-multiple-pipes-in-c?lq=1)

#####Input redirection
	check if there is `<` in the command
	open the inputfile
	redirection to the inputfile for the first command

#####Output redireciton
	check if there is `>` or `>>` in the command 
	`>` is for overwriting the current data in file
	`>>` is to append text to a file
	
#####FAQ in redirection: 	
1. [Redirection[Bash-hackers]](http://wiki.bash-hackers.org/syntax/redirection)	
2. [Cprogramming.com: Appending Text to Existing Text File](http://cboard.cprogramming.com/c-programming/103570-appending-text-existing-text-file.html)
3. Mode definition for open: [Stackoverflow: what does mode_t 0644 mean?](http://stackoverflow.com/questions/18415904/what-does-mode-t-0644-mean)

###`Signal`:
------------
Do not use `execvp()`

Use [getenv()](http://linux.die.net/man/3/getenv) to find PATH

Check if the file is executable using [access()](http://linux.die.net/man/2/access)

Use [execv()](http://linux.die.net/man/3/execv) or any syscall other than `execvp`

#####FAQ in	getting path and using exec:
1.`getenv` Introduction/instruction:  [Stackoverflow: Unix Path Searching C Function](http://stackoverflow.com/questions/147057/unix-path-searching-c-function)
2. [`execve` Syscall Family Nanual Page](http://linux.die.net/man/3/execve)
3. [Stackoverflow: System Call fork() and execv funtion](http://stackoverflow.com/questions/19147386/system-call-fork-and-execv-function)
4. If `getenv` does not return correct path the second time, check if the path variable has been modified by accident. [Stackoverflow: const char...changed?](http://stackoverflow.com/questions/10249750/const-char-changed)

#####Use Signal
When capture ^C, interrupt foreground job

Include `signal.h`

Use [raise()](http://linux.die.net/man/3/raise) and [signal()](http://linux.die.net/man/2/signal)
	
#####FAQ in `signal`:
1. Signal Introduction: [The Geek Stuff: Linux Signals - Example C Program to Catch Signals(SIGINT, SIGKILL, SIGSTOP)](http://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/)
2. All the signal handlers [YoLinux: C/C++ Signal Handling](http://www.yolinux.com/TUTORIALS/C++Signals.html)
3. [The GNU C Library: Signla Handling](http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_21.html)
4. [Stackoverflow: How to Kill a Child Process by the Parent Process?](http://stackoverflow.com/questions/6501522/how-to-kill-a-child-process-by-the-parent-process)
5. [Cprogramming: SIGNAL Suspend?](http://cboard.cprogramming.com/c-programming/121383-signal-suspended.html)

#####Implement `cd`
	Display current working directory 
	if `cd` is entered
	Change the current directory using [chdir()](http://linux.die.net/man/2/chdir)

#####FAQ in `cd`:
######Current Working Directory:
1. [Stackoverflow: Getting Absolute Path of a File](http://stackoverflow.com/questions/229012/getting-absolute-path-of-a-file)

######`chdir()`:
1. [Stackoverflow: Why is chdir() not successful?](http://stackoverflow.com/questions/16841365/why-is-chdir-not-successful?rq=1)
2. [Stackoverflow: chdir() - No Such File or Directory](http://stackoverflow.com/questions/13591065/chdir-no-such-file-or-directory?rq=1)

