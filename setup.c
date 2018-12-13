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

#define KEY 0xBEEFDEAD

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

void create() {
  int shmid = shmget(123456, 4, IPC_CREAT | 0666);
  int* data = shmat(shmid, 0, 0);
  if( *data == -1 ){
    printf("Error, %s\n", strerror(errno));
    exit(0);
  }
  *data = 0;
  if(shmdt(data) == -1){
    printf("Error, %s\n", strerror(errno));
    exit(0);
  }
  semget(KEY, 1, IPC_CREAT | 0666);
  int y = open("file.txt", O_CREAT, 0666);
  close(y);
}

void rem() {
  int shmid = shmget(123456, 4, IPC_CREAT | 0666);
  if( shmctl(shmid, IPC_RMID, NULL) == -1 ){
    printf("Error, %s\n", strerror(errno));
  }
  int semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if( semid == -1 ){
    printf("Error %d: %s\n", errno, strerror(errno));
  }
  if( semctl(semid, 0, IPC_RMID, 0) == -1){
    printf("Error %d: %s\n", errno, strerror(errno));
  }
  int f = fork();
  if(!f){
    char** command;
    command[0] = "cat";
    command[1] = "file.txt";
    if(execvp(command[0], command) == -1){
      printf("Error %d: %s\n", errno, strerror(errno));
    }
  }else{
    int status;
    wait(&status);
    char** command;
    command[0] = "rm";
    command[1] = "file.txt";
    if(execvp(command[0], command) == -1){
      printf("Error %d: %s\n", errno, strerror(errno));
    }
  }
}

void view() {
  char** command;
  command[0] = "cat";
  command[1] = "file.txt";
  if(execvp(command[0], command) == -1){
    printf("Error %d: %s\n", errno, strerror(errno));
  }
}

int main( int argc , char* argv[] ) {
  char* flag = argv[1];
  if ( strcmp(flag, "-c") == 0 ) {
    create();
  }
  if ( strcmp(flag, "-r") == 0 ) {
    rem();
  }
  if ( strcmp(flag, "-v") == 0 ) {
    view();
  }
  return 0;
}
