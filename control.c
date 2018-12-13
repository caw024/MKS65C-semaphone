#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
//Control

#define KEY 0xBEEFDEAD

 union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO */
 };

int main(int argc, char * argv[]) {
  int semid = semget(KEY, 1, 0644);
  if (semid == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    exit(0);
  }
  union semun us;
  us.val = 0;
  semctl(semid, 0, SETVAL, us);

  int shmid = shmget(123456, 4, 0666);
  int* data = shmat(shmid, 0, 0);
  if( *data == -1 ){
    printf("Error, %s\n", strerror(errno));
    exit(0);
  }

  FILE *fp = fopen("file.txt", "r");
  if(!fp){
    printf("Error, %s\n", strerror(errno));
    exit(0);
  }
  char* sentence = calloc(*data, sizeof(char));
  fseek(fp, *data * -1, SEEK_END);
  char c[1];
  while(!(feof(fp))){
    strcpy(c, fgetc(fp));
    strcat(sentence, c);
  }
  fclose(fp);

  printf("Last line in story: %s\n", sentence);
  printf("Enter the next line for the story\n");
  scanf("%[^\n]", sentence);

  *data = strlen(sentence);
  y = open("file.txt", O_WRONLY | O_APPEND, 0666);
  write(y, sentence, *data);
  close(y);

  us.val = 1;
  semctl(semid, 0, SETVAL, us);
  return 0;
}
