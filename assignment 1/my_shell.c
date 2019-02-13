/*What to replace here?????
 *questions to be asked in class have '???'
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dirent.h>
/*Can we use dirent.h ????*/

#include <fcntl.h>
/* Again.... Can we use fcntl.h ???? */
// cat not implemented

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// Helper functions
unsigned int stoi(char *token)
{
	unsigned int unsInt = 0;
	for (int i = 0; i < strlen(token); i++)
	{
		unsInt = unsInt * 10 + (token[i] - '0');
	}
	return unsInt;
}

/* Splits the string by space and returns the array of tokens */
char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++)
	{

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}

void echo(char **, int);
void pwd();
void exec(char *, char **, int);

void my_sleep(char **tokens, int i);

int main(int argc, char *argv[])
{
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;

	FILE *fp;
	if (argc >= 2)
	{
		fp = fopen(argv[1], "r");
		if (fp < 0)
		{
			printf("File doesn't exists.");
			return -1;
		}
	}
	int count = 8;
	int exit = 0;
	while (count-- && !exit)
	{
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if (argc == 2)
		{ // batch mode
			if (fgets(line, sizeof(line), fp) == NULL)
			{ // file reading finished
				break;
			}
			line[strlen(line) - 1] = '\0';
		}
		else
		{ // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		//do whatever you want with the commands, here we just print them

		for (i = 0; tokens[i] != NULL; i++)
		{
			printf("found token %s\n", tokens[i]);

			if (strcmp(tokens[i], "cd") == 0)
			{
				chdir(tokens[++i]); // done
			}

			else if (strcmp(tokens[i], "pwd") == 0)
			{
				exec("pwd", NULL, 0); // done
			}

			else if (strcmp(tokens[i], "ls") == 0)
			{
				char *array[1] = {tokens[++i]};
				exec("ls", array, 1); // done
			}

			else if (strcmp(tokens[i], "echo") == 0)
			{
				char *array[1] = {tokens[++i]};
				exec("echo", array, 1); // a little left
			}

			else if (strcmp(tokens[i], "sleep") == 0)
			{
				i++;
				// my_sleep(tokens, i);
				int sleepNo = stoi(tokens[i]);
				if (sleepNo > MAX_NUM_TOKENS)
				{
					printf("Shell: Incorrect command \n");
					// perror("Sleeping cannot be done for such long period \n");
					/*What to replace here?????*/
				}
				sleep(sleepNo); //think so left
			}
			else if (strcmp(tokens[i], "exit") == 0)
			{
				exit = 1;
				break; //done
			}

			else
			{
				printf("Shell: Incorrect command: %s\n", tokens[i]);
			}
		}

		// Freeing the allocated memory
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}

// void my_sleep(char **tokens, int i)
// {
// 	pid_t pid, wpid;
// 	int status;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		int sleepNo = stoi(tokens[i + 1]);
// 		if (sleepNo > MAX_NUM_TOKENS)
// 		{
// 			printf("Shell: Incorrect command \n");
// 			// perror("Sleeping cannot be done for such long period \n");
// 			/*What to replace here?????*/
// 		}
// 		sleep(sleepNo);
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pid)
// 	{
// 		wpid = waitpid(pid, &status, 0);
// 		printf("%d reaped \n", wpid);
// 	}

// 	else if (pid < 0)
// 	{
// 		perror("error forking");
// 	}
// }

void exec(char *command, char **args, int size)
{
	pid_t pid, wpid;
	int status;
	int i;
	char *new_arg[size + 2];
	new_arg[0] = command;
	for (i = 0; i < size; i++)
	{
		new_arg[i + 1] = args[i];
	}
	new_arg[i + 1] = NULL;

	pid = fork();
	if (pid == 0)
	{
		execvp(command, new_arg);
	}
	else if (pid)
	{
		wpid = waitpid(pid, &status, 0);
		printf("%d reaped \n", wpid);
	}
}