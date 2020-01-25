#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

#define DICTIONARY_ARRAY_SIZE 20
#define CHAR_SIZE 100
#define KEY 2684

typedef int bool;
#define true 1
#define false 0

typedef struct {
    char pl[CHAR_SIZE];
    char en[CHAR_SIZE];
} dictionary;

typedef struct {
    long PID;
    char word[CHAR_SIZE];
} message;

dictionary dict[DICTIONARY_ARRAY_SIZE] = {
    {"samochod", "car"}, {"portfel", "wallet"},
    {"kot", "cat"}, {"ksiazka", "book"},
    {"lato", "summer"}, {"woda", "water"},
    {"papier", "paper"}, {"stol", "table"},
    {"mysz", "mouse"}, {"klawiatura", "keyboard"}
};

char *translate(char*);

int queue_in, queue_out;
int main(int argc, char *argv[]) {
   message msg;
   queue_in = msgget(KEY, 0777 | IPC_CREAT);
   queue_out = msgget(KEY, 0777 | IPC_CREAT);

   while (true) {
       if (msgrcv(queue_in, &msg, sizeof(char) * 110, 0, 0) == -1) {
           printf("ERROR: Can't get the message from client\n");
           exit(1);
       }

       printf("Recieved: \"%s\", ", msg.word);
       strcpy(msg.word, translate(msg.word));
       printf("sending to PID[%ld] response: \"%s\".\n", msg.PID, msg.word);

       if(msgsnd(queue_out, &msg, sizeof(char) * 110, 0) == -1){
           printf("ERROR: Can't send the message to client\n");
           exit(2);
       }
   }

   return 0;
}

char *translate(char* word){
    int i;
    for ( i = 0; i < DICTIONARY_ARRAY_SIZE; i++) {
        if(!strcmp(word, dict[i].pl))
        return dict[i].en;
    }

    return "word not found";
}
