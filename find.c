#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
  char *p;
  for(p=path+strlen(path);p>=path&&*p!='/';p--)
  ;
  p++;
  return p;  
}

void find(char *path,char *target)
{
  char buf[100],*p;
  int fd;
  struct dirent de;
  struct stat st;
  fd=open(path,0);
  fstat(fd,&st);
  printf("open_%s_%d\n",path,fd);
  if(st.type==T_FILE)
  {
    if(strcmp(fmtname(path),target)==0)
    {
      printf("%s\n",path);
    }
  }
  else if(st.type==T_DIR)
  {
    strcpy(buf,path);
    p=buf+strlen(buf);
    *p++='/';
    while(read(fd,&de,sizeof(de))==sizeof(de))
    {
      if(de.inum==0||strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
      {
        continue;
      }
      memmove(p,de.name,DIRSIZ);
      p[DIRSIZ]=0;
      find(buf,target);
    }
  }
  if(fd>=14)
  {  
  printf("close_%s_%d\n",path,fd);
  close(fd);
  }
  return;
}

int main(int argc,char *argv[])
{
  if(argc!=3)
  {
    printf("fail_to_find\n");
    exit(0);
  }
  find(argv[1],argv[2]);
  exit(0);
}
