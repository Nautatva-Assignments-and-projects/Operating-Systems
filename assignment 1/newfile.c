#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
int main(int argc, char **argv)
{
  struct dirent **namelist;
  int n;
  if (argc < 1)
  {
    printf("1");
    exit(EXIT_FAILURE);
  }
  else if (argc == 1)
  {
    printf("2");
    n = scandir(".", &namelist, NULL, alphasort);
  }
  else
  {
    printf("3");
    n = scandir(argv[1], &namelist, NULL, alphasort);
  }
  if (n < 0)
  {
    printf("4");
    perror("scandir");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("5");
    while (n--)
    {
      printf("%s \n", namelist[n]->d_name);
      free(namelist[n]);
    }
    free(namelist);
  }
  exit(EXIT_SUCCESS);
}
