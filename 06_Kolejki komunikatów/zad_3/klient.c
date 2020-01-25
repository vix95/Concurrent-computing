#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>

#define CHAR_SIZE 100
#define KEY 2684

typedef struct {
    char pl[CHAR_SIZE];
    char en[CHAR_SIZE];
} dictionary;

typedef struct {
    long PID;
    char word[CHAR_SIZE];
} message;

int main(int argc, char *argv[]) {
    message msg;
    int queue_in, queue_out;

    queue_in = msgget(KEY, 0777);
    queue_out = msgget(KEY, 0777);

    strcpy(msg.word, argv[1]);
    msg.PID = getpid();

    if(msgsnd(queue_in, &msg, sizeof(char) * 110, 0) == -1) {
        printf("ERROR: Can't send the message to server\n");
        exit(1);
    }

    printf("\"%s\" = ", argv[1]);

    if(msgrcv(queue_out, &msg, sizeof(char) * 110, getpid(), 0) == -1) {
        printf("ERROR: Can't get the message from server\n");
        exit(2);
    }

  printf("\"%s\"\n", msg.word);

  return 0;
}
