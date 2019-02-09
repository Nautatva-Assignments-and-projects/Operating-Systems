#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
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
					printf("Did not find \" \n");
					break;
				}
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
