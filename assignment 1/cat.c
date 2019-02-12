#include <fcntl.h>
#include <unistd.h>

static int
cat_fd(int fd)
{
  char buf[4096];
  ssize_t nread;

  while ((nread = read(fd, buf, sizeof buf)) > 0)
  {
    ssize_t ntotalwritten = 0;
    while (ntotalwritten < nread)
    {
      ssize_t nwritten = write(STDOUT_FILENO, buf + ntotalwritten, nread - ntotalwritten);
      if (nwritten < 1)
        return -1;
      ntotalwritten += nwritten;
    }
  }

  return nread == 0 ? 0 : -1;
}

static int
cat(const char *fname)
{
  int fd, success;

  if ((fd = open(fname, O_RDONLY)) == -1)
    return -1;

  success = cat_fd(fd);

  if (close(fd) != 0)
    return -1;

  return success;
}

int main(int argc, char **argv)
{
  int i;

  if (argc == 1)
  {
    if (cat_fd(STDIN_FILENO) != 0)
      goto error;
  }
  else
  {
    for (i = 1; i < argc; i++)
    {
      if (cat(argv[i]) != 0)
        goto error;
    }
  }
  return 0;

error:
  write(STDOUT_FILENO, "error\n", 6);
  return 1;
}