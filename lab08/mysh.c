// mysh.c ... a minimal shell
// Started by John Shepherd, October 2017
// Completed by <<YOU>>, July 2019

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PATH "/bin:/usr/bin"
#define DBUG

static void execute (char **, char **, char **);
static bool isExecutable (char *);
static char **tokenise (const char *, const char *);
static void freeTokens (char **);
static char *trim (char *);
static size_t strrspn (const char *, const char *);


// Environment variables are pointed to by `environ', an array of
// strings terminated by a NULL value -- something like:
//     { "VAR1=value", "VAR2=value", NULL }
extern char **environ;

int main (int argc, char *argv[])
{
	// grab the `PATH' environment variable;
	// if it isn't set, use the default path defined above
	char *pathp;
	if ((pathp = getenv ("PATH")) == NULL)
		pathp = DEFAULT_PATH;
	char **path = tokenise (pathp, ":");

#ifdef DBUG
	for (int i = 0; path[i] != NULL; i++)
		printf ("dir[%d] = %s\n", i, path[i]);
#endif

	// main loop: print prompt, read line, execute command
	char line_[BUFSIZ];
	printf ("mysh$ ");
	while (fgets (line_, BUFSIZ, stdin) != NULL) {
		char *line = trim (line_); // remove leading/trailing space
		if (strcmp (line, "exit") == 0) break;
		if (strcmp (line, "") == 0) { printf ("mysh$ "); continue; }

		pid_t pid;   // pid of child process
		int stat;	 // return status of child

		// tokenise 
		char **args = tokenise (line, " ");
		
		// fork
		pid = fork();
		if (pid != 0) {  // parent
		   wait(&stat); 
		
		// cleanup
		   freeTokens(args);
		}
		else {           // child
		// execute
		   if ((strcmp(args[0], "cd") == 0)) {
		    if (args[1] != NULL) {
		        chdir(args[1]);
		    }
		    else {
		        chdir(getenv("HOME"));
		    }
		   }
		   else if ((strcmp(args[0], "pwd") == 0)){
		    char str[BUFSIZ];
		    printf("%s\n", getcwd(str, BUFSIZ));
		   }
		   else {
		    execute(args, path, environ);
		   }
        }
		printf ("mysh$ ");
	}
	printf ("\n");

	freeTokens (path);

	return EXIT_SUCCESS;
}



// execute: run a program, given command-line args, path and envp
static void execute (char **args, char **path, char **envp)
{
    char str[BUFSIZ];
    char command[BUFSIZ];
    strcpy(command, args[0]);
    int flag = 0;
/**
args = tokenise the command line
if args[0] starts with '/' or '.':
    check if the file called args[0] is executable;
    if so, use args[0] as the command.
    **/
    if (args[0][0] == '/' || args[0][0] == ' ') {
        if (isExecutable (args[0])) {
            strcpy(command, args[0]);
            flag = 1;
        }
    }

/**
otherwise:
    for each of the directories D in the path,
        check if an executable file called "D/args[0]" exists;
        if it does, use that file name as the command.
    **/
    else {
        for (int i = 0; path[i] != NULL; i++) {
             strcpy(str, path[i]);
             strcat(str, "/");
             strcat(str, args[0]);
             if (isExecutable(str)) {
                strcpy(command, str);
                flag = 1;
                break;
            }
        }
    }
/**
if no executable file was found,
    print a 'command not found' message,
    and exit this child process.
    **/
    if (flag == 0) {
        printf("%s: Command not found\n", command);
    }
/**

otherwise:
    print the full name of the command being executed;
    use execve() to attempt to run the command with args and envp,
    if it didn't run, print an error,
    and exit this child process (as execute() is not supposed to return)
**/
    else {
        printf ("Executing %s\n", command);
        execve (command, args, envp);
        printf("error\n"); 
    }

	/// TODO: implement the `find-the-executable and execve(3) it' code
}

/// isExecutable: check whether this process can execute a file
static bool isExecutable (char *cmd)
{
	struct stat s;
	// must be accessible
	if (stat (cmd, &s) < 0) return false;
	// must be a regular file
	if (! S_ISREG (s.st_mode)) return false;
	// if it's owner executable by us, ok
	if (s.st_uid == getuid () && s.st_mode & S_IXUSR) return true;
	// if it's group executable by us, ok
	if (s.st_gid == getgid () && s.st_mode & S_IXGRP) return true;
	// if it's other executable by us, ok
	if (s.st_mode & S_IXOTH) return true;
	// otherwise, no, we can't execute it.
	return false;
}

/// tokenise: split a string around a set of separators
/// create an array of separate strings
/// final array element contains NULL
static char **tokenise (const char *str_, const char *sep)
{
	char **results  = NULL;
	size_t nresults = 0;

	// strsep(3) modifies the input string and the pointer to it,
	// so make a copy and remember where we started.
	char *str = strdup (str_);
	char *tmp = str;

	char *tok;
	while ((tok = strsep (&str, sep)) != NULL) {
		// "push" this token onto the list of resulting strings
		results = realloc (results, ++nresults * sizeof (char *));
		results[nresults - 1] = strdup (tok);
	}

	results = realloc (results, ++nresults * sizeof (char *));
	results[nresults - 1] = NULL;

	free (tmp);
	return results;
}

/// freeTokens: free memory associated with array of tokens
static void freeTokens (char **toks)
{
	for (int i = 0; toks[i] != NULL; i++)
		free (toks[i]);
	free (toks);
}

/// trim: remove leading and trailing whitespace from a string
static char *trim (char *str)
{
	char *space = " \r\n\t";
	str[strrspn (str, space) + 1] = '\0'; // skip trailing whitespace
	str = &str[strspn (str, space)];      // skip leading whitespace
	return str;
}

/// strrspn: find a suffix substring made up of any of `set'.
/// like `strspn(3)', except from the end of the string.
static size_t strrspn (const char *str, const char *set)
{
	size_t len;
	for (len = strlen (str); len != 0; len--)
		if (strchr (set, str[len]) == NULL)
			break;
	return len;
}
