This shell program is designed to run in C. 



To compile, enter the terminal and navigate to the directory of the files, then use the provided makefile by typing "make". 

Or to compile manually, use the following command:
g++ -Wall -ulimit -Wextra -o myshell myshell.c

Lasty, to run the program, type "./myshell"



Commands are ran line by line via the command line interface.
Supported Commands:

       C file1 file2       Copy; create file2 then copy all contents of file1 to file2
       D file              Delete the named file.
       E comment           Echo; display comment on screen followed by a new line
       H                   Help; display the user manual.
       L                   List the contents of the current directory.
       M file              Make; create the named text file by launching the nano text editor.
       P file              Print; display the contents of the named file on screen.
       Q                   Quit the shell.
       S                   Surf the web by launching firefox as a background process.
       W                   Wipe; clear the screen.\n"
       V file              Vim; Create the named text file through Vim.

Besides this, it supports other typical executable UNIX commands, and external programs can be launched.



Works Cited:
    Linux man-pages: https://www.kernel.org/doc/man-pages/
    GNU Make: https://www.gnu.org/software/make/manual/
    Stack Overflow: https://stackoverflow.com/questions/4204915/please-explain-the-exec-function
    -and-its-family
