#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

//Control

#define KEY 0xDEADBEEF

 union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO */
 };

int main(int argc, char * argv[]) {

  //create
  if (strcmp(argv[0],"-c") == 0){

    //Mr. K's code
    int semd;
    int r;
    int v;
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
      printf("error %d: %s\n", errno, strerror(errno));
      semd = semget(KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
      printf("semctl returned: %d\n", v);
    }
    else {
      union semun us;
      us.val = 3;
      r = semctl(semd, 0, SETVAL, us);
      printf("semctl returned: %d\n", r);
    }

  }

  //view
  if (strcmp(argv[0],"-v") == 0){
    int fd = open("file.txt", O_RDONLY | O_CREAT);
    char * r = calloc(sizeof(char),100);
    int red = read(fd,r,100);
    printf("\nDid it read? %d",red);

    printf("Story: %s\n", r);
    close(fd);

  }

  //remove
  if (strcmp(argv[0],"-r") == 0){}

    
    return 0;
}
