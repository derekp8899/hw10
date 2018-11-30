/*
Derek Popowski
derek.a.popowski@und.edu
CSci 451 Operating Systems
HW10
12-3-18

program2.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
char *formatWord(char* buffer);//to format words read in from the pipe
int main(int argc, char* argv[]){

  char *pipe1read = argv[1];
  char *pipe2write = argv[2];
  char *sem1Id = argv[3];
  char *sem2Id = argv[4];
  char *shmId = argv[5];
  int p1R = atoi(pipe1read);
  int p2W = atoi(pipe2write);
  sem_t *sem1 = sem_open(sem1Id,O_CREAT);
  sem_t *sem2 = sem_open(sem2Id,O_CREAT);
  char *buffer = malloc(100);
  int *spoint;
  spoint = shmat(atoi(shmId),NULL,0);
  *spoint=0;
  printf("starting program2\n\n");
  // int file = open("temp1.data",O_RDONLY);//open the input file
  sem_wait(sem1);  
  //read(p1R,buffer,100);//read the data in from the file
  //sem_post(sem1);
  char *word = malloc(250);//word temp storage buffer
  system("touch temp2.data");//create the temp1.data file
  int temp2 = open("temp2.data",O_WRONLY);//open temp1 for writing
  //  word = strtok(buffer,"\n");//grab first word from the input buffer
  char *word2 = malloc(250);//word buffer for formatting
  printf("program2\n\n");
  //printf("%s",buffer);
  //sem_wait(sem1);
  while(read(p1R,buffer,100) > 0){
    //sem_post(sem1);
    printf("%s",buffer);
    write(temp2,buffer,strlen(buffer));
    word = formatWord(buffer);
    write(p2W,word,100);
    (*spoint)++;
    sem_post(sem2);
    int semval;
    sem_getvalue(sem1,&semval);
    if(semval==0)
      sleep(.2);
    //sem_wait(sem1);
  }
  printf("reading complete\n");

  //  printf("%s",word);//verify the word
  /* int share1= 0;int share2 = 0;//counters for each 'type'
  while (word != NULL){//while there are still words
    if(word[0] == 65||word[0] == 69||word[0] == 73||word[0] == 79||word[0] == 85||word[0] == 97||word[0] == 101||word[0] == 105||word[0] == 111||word[0] == 117){//if word starts with a vowel (upper or lower)
      sprintf(word2,"%sray\n",word);//format the word
      share1++;//inc shared1
    }
    else{

      int i = 0;

      char temp,temppunc;
      temp = word[0];
      temppunc = word[strlen(word)-1];//temp store first and last characters for formatting

      for (i = 1; i < (strlen(word)); i++){//loop word and shift
	
	if(word[i] < 65)//if we find a puncuation symbol
	  break;
	word[i-1] = word[i];//shift the word
	
      }

      if(temppunc < 65){// if there was punctuation at the end of the word
	word[strlen(word)-2] = temp;//move frist character to the end
	word[strlen(word)-1] = '\0';//null term
	sprintf(word2,"%say%c\n",word,temppunc);//format the word adding back the punctuation
      }
      else{//no punctuation
	word[strlen(word)-1] = temp;//move frist character to the end
	sprintf(word2,"%say\n",word,temppunc);//format the word
      }
      share2++;//inc shared2
    }
    write(temp2,word2,strlen(word2));//write to temp1.data
    //printf("%s",word2);//verify the word
    word = strtok(NULL,"\n");//grab the next word
  }
  
  //  printf("share counts %d %d \n",share1,share2);//verify the counters
  /*
  int shared1 = open("shared1.dat",O_WRONLY);
  sprintf(sharedbuf,"%d\n",share1);
  write(shared1,sharedbuf,strlen(sharedbuf));//write count of shared1
  int shared2 = open("shared2.dat",O_WRONLY);
  sprintf(sharedbuf,"%d\n",share2);
  write(shared2,sharedbuf,strlen(sharedbuf));//write count of shared2
  */
  close(p1R);sem_close(sem1);
  shmdt(spoint);
  free(buffer);free(word);free(word2);//free(sharedbuf);//free the string buffers
  return(0);
  //printf("temp2.data written\nshared1.dat written\nshared2.dat written\n");

}
char *formatWord(char *buffer){

  char* word = malloc(100);
  word = strtok(buffer,"\n");//grab word from the input buffer
  char* word2 = malloc(100);
  if(word[0] == 65||word[0] == 69||word[0] == 73||word[0] == 79||word[0] == 85||word[0] == 97||word[0] == 101||word[0] == 105||word[0] == 111||word[0] == 117){//if word starts with a vowel (upper or lower)
    sprintf(word2,"%sray\n",word);//format the word
    //    share1++;//inc shared1
  }
  else{
    
    int i = 0;
    
    char temp,temppunc;
    temp = word[0];
    temppunc = word[strlen(word)-1];//temp store first and last characters for formatting
    
    for (i = 1; i < (strlen(word)); i++){//loop word and shift
      
      if(word[i] < 65)//if we find a puncuation symbol
	break;
      word[i-1] = word[i];//shift the word
      
    }
    
    if(temppunc < 65){// if there was punctuation at the end of the word
      word[strlen(word)-2] = temp;//move frist character to the end
      word[strlen(word)-1] = '\0';//null term
      sprintf(word2,"%say%c\n",word,temppunc);//format the word adding back the punctuation
    }
    else{//no punctuation
      word[strlen(word)-1] = temp;//move frist character to the end
      sprintf(word2,"%say\n",word,temppunc);//format the word
    }
    //    share2++;//inc shared2
  }
  printf("%s",word2);//print the word to verify
  return word2;
}
