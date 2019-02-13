/*What to replace here?????
 *questions to be asked in class have '???'
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

char *processecho(char **, int);
void pwd();
int exec(char *, char **, int);

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
		// printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		//do whatever you want with the commands, here we just print them

		for (i = 0; tokens[i] != NULL; i++)
		{
			// printf("found token %s\n", tokens[i]);

			if (strcmp(tokens[i], "cd") == 0)
			{ // done
				chdir(tokens[++i]);
			}

			else if (strcmp(tokens[i], "exit") == 0)
			{ //done
				exit = 1;
				break;
			}

			//

			//

			// problem
			else if (strcmp(tokens[i], "sleep") == 0)
			{
				i++;
				int sleepNo = stoi(tokens[i]);
				if (sleepNo > MAX_NUM_TOKENS)
				{
					printf("Shell: Incorrect command \n");
				}
				sleep(sleepNo);
			}

			//

			//

			// exec waale commands
			else if (strcmp(tokens[i], "pwd") == 0)
			{ // done
				exec("pwd", NULL, 0);
			}

			else if (strcmp(tokens[i], "date") == 0)
			{ // done
				exec("date", NULL, 0);
			}

			else if (strcmp(tokens[i], "ps") == 0)
			{ // done
				exec("ps", NULL, 0);
			}

			else if (strcmp(tokens[i], "ls") == 0)
			{ //done
				if (tokens[i++] != NULL)
				{
					char *array[1] = {tokens[i]};
					i += exec("ls", array, 1);
				}
				else
				{
					exec("ls", NULL, 0);
				}
			}

			else if (strcmp(tokens[i], "cat") == 0)
			{ //done
				if (tokens[i++] != NULL)
				{
					char *array[1] = {tokens[i]};
					i += exec("cat", array, 1);
				}
			}

			else if (strcmp(tokens[i], "echo") == 0)
			{ //done - check for size though
				char *string = processecho(tokens, i);
				char *array[1] = {string};
				exec("echo", array, 1);
				break;
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

char *processecho(char **tokens, int tokenNo)
{

	int start = 0;
	int stop = 0;
	static char word[1024]; //use malloc later part
	int pointer = 0;
	int length = 0;

	// tokenNo contains the word echo;
	for (int i = tokenNo + 1; tokens[i] != NULL; i++)
	{
		char *currentWord = tokens[i];
		for (int j = 0; j < strlen(currentWord); j++)
		{
			char readchar = currentWord[j];
			// Handling encounter with "
			if (readchar == '"')
			{
				// first encounter
				if (start == 0)
				{
					start = 1;
					continue;
				}
				// last encounter
				else
				{
					stop = 1;
					// word[1024] = length + '0';
					// printf("\n");
					word[pointer] = '\0';
					return word;
					// last bit is the number of words
				}
			}
			// if immediately, we dont find ", give error
			else if (start == 0)
			{
				return NULL;
				// error for did not find "
			}
			else
			{
				word[pointer++] = readchar;
			}
		}
		word[pointer++] = ' ';
		length++;
	}
	word[pointer] = '\0';
	return word;
	// 0 for loop did not end
}

int exec(char *command, char **args, int size)
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
		// printf("%d reaped \n", wpid);
	}
	return size;
}