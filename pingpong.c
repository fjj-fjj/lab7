#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc,char *argv[])
{
  char buf[20];
  int pfd[2];
  int cfd[2];
  pipe(pfd);
  pipe(cfd);
  if(fork()==0)
  {
    close(cfd[0]);
    strcpy(buf,"received pong");
    write(cfd[1],buf,sizeof(buf));
    close(pfd[1]);
    read(pfd[0],buf,20);
    printf("%d: %s\n",getpid(),buf);
    exit(0);
  }
  else
  {
    close(pfd[0]);
    strcpy(buf,"received ping");
    write(pfd[1],buf,sizeof(buf));
    wait(0);
    close(cfd[1]);
    read(cfd[0],buf,20);
    printf("%d: %s\n",getpid(),buf);
  }
  return 0;
}
