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

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **tokenize(char *line);
int echo(char **tokens, int tokenNo);
unsigned int stoi(char *token);
int currentwd();
void ls();

int main(int argc, char *argv[])
{
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;

	FILE *fp;
	if (argc == 2)
	{
		fp = fopen(argv[1], "r");
		if (fp < 0)
		{
			printf("File doesn't exists.");
			return -1;
		}
	}

	while (1)
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

			if (strcmp(tokens[i], "echo") == 0)
			{
				i = echo(tokens, i);
				// i is the token number at which the echo trails off
				if (i == -1)
				{

					printf("Shell: Incorrect command \n");
					// perror("Did not find \" \n");
					/*What to replace here?????*/
					break;
				}
			}

			else if (strcmp(tokens[i], "sleep") == 0)
			{
				int sleepNo = stoi(tokens[i + 1]);
				if (sleepNo > MAX_NUM_TOKENS)
				{
					printf("Shell: Incorrect command \n");
					// perror("Sleeping cannot be done for such long period \n");
					/*What to replace here?????*/
					break;
				}
				sleep(sleepNo);
			}

			else if (strcmp(tokens[i], "cd") == 0)
			{
				i++;
				if (chdir(tokens[i]) == -1)
				/*We just have to change directory in our custom shell right!??? */
				{
					printf("Shell: Incorrect command \n");
					// perror("no such directory \n");
					/*What to replace here????? */
				}
				else
				{
					currentwd();
					/*What to replace here????? */
				};
			}

			else if (strcmp(tokens[i], "ls") == 0)
			{
				ls();
				/*Work on this ????*/
			}
			else
			{
				printf("Shell: Incorrect command \n");
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

int echo(char **tokens, int tokenNo)
{

	int start = 0;
	int stop = 0;
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
					printf("\n");
					return i;
					// i returns the word index last echoed
				}
			}
			// if immediately, we dont find ", give error
			else if (start == 0)
			{
				return -1;
				// error for did not find "
			}
			else
			{
				printf("%c", readchar);
			}
		}
		printf(" ");
	}

	return 0;
	// 0 for loop did not end
}

unsigned int stoi(char *token)
{
	unsigned int unsInt = 0;
	for (int i = 0; i < strlen(token); i++)
	{
		unsInt = unsInt * 10 + (token[i] - '0');
	}
	return unsInt;
}

// function to check the current working directory
int currentwd()
{
	char cwd[MAX_INPUT_SIZE];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("Current working dir: %s\n", cwd);
	}
	else
	{
		perror("getcwd() error");
		return 1;
	}
	return 0;
}

void ls()
{
	struct dirent *de; // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir(".");

	if (dr == NULL) // opendir returns NULL if couldn't open directory
	{

		printf("Shell: Incorrect command \n");
		// printf("Could not open current directory");
		return;
	}

	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
	// for readdir()
	while ((de = readdir(dr)) != NULL)
		printf("%s\n", de->d_name);

	closedir(dr);
	printf("is using direct.h header okay??? \n");
	return;
}