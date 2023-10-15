// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: George McCannon Date: Oct 15, 2023

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN		16
#define MAX_LINE_LEN	80
#define WHITESPACE		" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};


/*
   structures for the lookup table to make the command names custom
*/

struct command_mapping {
   const char *name;
   const char *executable;
};

struct command_mapping commandMap[] = {
    {"C", "cp"},
    {"D", "rm"},
    {"E", "echo"},
    {"L", "ls"},
    {"M", "nano"},
    {"S", "firefox"},
    {"W", "clear"},
    {"P", "more"},
    {"X", NULL},
    {"V", "vim"},
};

/*
   end lookup table
*/


/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void printHelp();

int main(int argc, char *argv[]) {

   if(argc){};
   if(argv){};

   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;
   const char *executable = NULL; //character used for the lookup table

   while (true) 
   {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      /* 
        Custom Commands not using execvp
        If one of the following commands is used, do not fork,
        break or go to the top of the loop
      */
      
      // Check if the command is "Q" and exit the shell
      if (strcmp(cmdLine, "Q") == 0) 
      {
         printf("Exiting the shell.\n");
         break;
      }
      
      // Check if the command is "H" and print the help page
      if (strcmp(cmdLine, "H") == 0) 
      {
         printHelp();
         continue;
      }
      
      /*
        end custom commands
      */
      
	  
      // Look up the command in the mapping table
	for (long unsigned int i = 0; i < sizeof(commandMap) / sizeof(commandMap[0]); i++) {
    	   if (strcmp(cmdLine, commandMap[i].name) == 0) 
    	   {
              if (commandMap[i].executable == NULL) 
              {
                 // Handle the custom X command, use the argument as the executable
                 executable = command.argv[1]; 
              } 
              else 
              {
                 executable = commandMap[i].executable;
              }
    	   }
	}
	  
      // Check if the command is not in the lookup table then run execvp
      // if yes, attempt to run as normal with the custom command
      if (executable == NULL) 
      {
         // Create a child process to execute the command
	 if ((pid = fork()) == 0) 
	 {
            // Child executing command
            // Handle the case where execvp fails (invalid command)
            // If execvp is successful, run as normal, if not, kill the child
            if (execvp(command.name, command.argv) == -1) 
            {
               printf("Invalid Command: %s\n", command.name);
               exit(1);
            }
         }
      }
      else
      {
         // Create a child process to execute the command
	 if ((pid = fork()) == 0) 
	 {
            // Child executing command
            // Handle the case where execvp fails (invalid components)
            // If execvp is successful, run as normal, if not, kill the child
            if (execvp(executable, command.argv) == -1) 
            {
               perror("execvp");
               exit(1);
            }
         }
      }
      
      //end command run conditions
      
      /*
      set executable variable to null to prevent a bug where invalid 
      commands executed after vaild commands are ran as the valid command
      */
      executable = NULL; 
      
      // Wait for the child to terminate
      wait(&status);
   }
   
   /* Shell termination */
   printf("\n\nshell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function *///////

/* Print help guide, from OS shell PDF */
void printHelp()
{
   printf("\nList of commands: \n"
      "C file1 file2       Copy; create file2 then copy all contents of file1 to file2\n"
      "D file              Delete the named file.\n"
      "E comment           Echo; display comment on screen followed by a new line\n"
      "H                   Help; display the user manual.\n"
      "L                   List the contents of the current directory.\n"
      "M file              Make; create the named text file by launching the nano text editor.\n"
      "P file              Print; display the contents of the named file on screen.\n"
      "Q                   Quit the shell.\n"
      "S                   Surf the web by launching firefox as a background process.\n"
      "W                   Wipe; clear the screen.\n"
      "V file              Vim; Create the named text file through Vim.\n\n"
          );
}


/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char promptString[] = "gwm13@Linux-os";
   char cwd[MAX_LINE_LEN];
   
   printf("%s:~", promptString);
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s$ ", cwd);
   }
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */
