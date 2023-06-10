#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MAX 36

void pw(int *fd)
{
  int n,i;
  read(fd[0],&n,sizeof(int));
  if(n>=MAX)
  return;
  int fd1[2];
  pipe(fd1);
  printf("prime %d\n",n);
  if(fork()!=0)
  {
    close(fd1[0]);
    while(read(fd[0],&i,sizeof(int))&&i!=MAX)
    {
      if(i%n!=0)
      {
        write(fd1[1],&i,sizeof(int));
      }      
    }
    write(fd1[1],&i,sizeof(int));
    wait(0);
  }
  else
  {
    close(fd1[1]);
    pw(fd1);
    exit(0);
  }
}

int main(int argc,char *argv[])
{
  int fd[2];
  int num=2,i;
  pipe(fd);
  if(fork()!=0)
  {
    close(fd[0]);
    printf("prime %d\n",num);
    for(i=num;i<MAX;i++)
    {
      if(i%num!=0)
      { 
        write(fd[1],&i,sizeof(int));
      }
    }
    write(fd[1],&i,sizeof(int));
    wait(0);
  }
  else
  {
    close(fd[1]);
    pw(fd);
    exit(0);
  }
  return 0;
}
