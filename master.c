/*
Derek Popowski
derek.a.popowski@und.edu
CSci 451 Operating Systems
HW10
12-3-18

master.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

struct types{//struct for storing of type1 and type2 word counts

  int type1;
  int type2;

};

int main(int argc, char * argv[]){
  
    if(argc < 2){
    printf("ERROR no file names supplied \nUsage: ./master infile.txt outfile.txt\n");
    return(0);
    }
    char *inName = argv[1];
    char *outName = argv[2];
  
  system("echo dpopowski > key.txt");//key file for use with ftok
  struct types counters;
  int pipe1[2];//pipe FDs
  int pipe2[2];
  pipe(pipe1);
  pipe(pipe2);//open the pipes
  char* p1R = malloc(20);
  char* p1W = malloc(20);
  sprintf(p1R,"%d",pipe1[0]);
  sprintf(p1W,"%d",pipe1[1]);//set pipe end FDs to strings
  char* p2R = malloc(20);
  char* p2W = malloc(20);
  sprintf(p2R,"%d",pipe2[0]);
  sprintf(p2W,"%d",pipe2[1]);//set pipe2 end FDs to strings
  counters.type1 = 12;
  counters.type2 = 44;
  char *shmKey = malloc(20);
  int shmintkey = 4321;
  char *semKey1 = "/derek1";
  char *semKey2 = "/derek2";
  sem_open(semKey1,O_CREAT,0666,0);
  sprintf(shmKey,"%d",shmintkey);//create shmkey string for passing to program2 and 3 
  key_t shmkey = ftok("key.txt",atoi(shmKey));//create the SHM key

  int shmid = shmget(shmkey, sizeof(struct types), IPC_CREAT | 0666);//generate the shared memory

  if(shmid<0){
    printf("error\n");
    exit(1);

  }
  printf("%d\n",shmid);
  struct types *spoint;
  spoint = shmat(shmid,NULL,0);

  //  spoint->type1 = 11;
  memcpy(spoint,&counters,sizeof(struct types));
  //printf("%d\n",spoint->type1);
  //printf("%d\n",spoint->type2);
  shmdt(spoint);
  close(pipe1[0]);//close all the pipes
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
    pid_t pid1,pid2;
  pid1 = fork();
  if(pid1 == 0){
    printf("forking p1\n");
    char *args[] = {"program1",inName,p1W,semKey1,NULL};
    execv("./program1",args);
  }
  else{
    sleep(.2);
    
    pid2 = fork();
    if(pid2 == 0){
      printf("forking p2\n");
      char *args[] = {"program2",p1R,p2W,semKey1,NULL};
      if(execv("./program2",args) < 0){
	printf("execv error p2\n");
	printf("%d errno\n",errno);
      }
      
    }
    
  }
  //int status;
  wait(NULL);
  //printf("program1 exit\n");
  //char buffer[100];
  //fflush(stdout);
  //int p1read = atoi(p1R);
  //read(p1read,buffer,100);
  //printf("%s\n",buffer);

  wait(NULL);
  // char *string = "pipe string";
  //int p1write = atoi(p1W);
  //write(p1write,string,strlen(string)+1);
  shmctl(shmid, IPC_RMID, NULL);//remove the shared memory
  sem_unlink(semKey1);
}
