#i#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
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
  int val = semctl(semid, 0, GETVAL, 0);
  if( val == 1 ){
    printf("The game is currently being played by another user. Please wait your turn\n");
    while( val = semctl(semid, 0, GETVAL, 0) );
  }
  union semun us;
  us.val = 1;
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
  printf("Last line in story: ");
  fseek(fp, *data * -1, SEEK_END);
  int c;
  while(1){
    c = fgetc(fp);
    if( feof(fp) || c == 10 ){
      break;
    }
    printf("%c", c);
  }
  fclose(fp);

  printf("\nEnter the next line for the story\n");
  char* sentence = calloc(*data, sizeof(char));
  scanf("%[^\n]", sentence);
  strcat(sentence, "\n");

  *data = strlen(sentence);
  int y = open("file.txt", O_WRONLY | O_APPEND, 0666);
  write(y, sentence, *data);
  close(y);

  us.val = 0;
  semctl(semid, 0, SETVAL, us);
  return 0;
}
