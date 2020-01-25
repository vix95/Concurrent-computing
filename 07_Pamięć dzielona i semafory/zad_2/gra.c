#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define key 47 // IPC key
#define semaphore_1 0
#define semaphore_2 1

typedef int bool;
#define true 1
#define false 0

int memory;
int semafors;
int board[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};
char *playername;
int Player1 = 1;
int Player2 = 2;
int *shared_memory; // shared_memory = shared memory
int result = 0; // 0 - nothing, 1 - Player1, 2 - Player2, 3 - remis

struct sembuf Player1_lock = {semaphore_2, -1, 0};  // lock P1
struct sembuf Player1_unlock = {semaphore_1, 1, 0}; // unlock P1
struct sembuf Player2_lock = {semaphore_1, -1, 0};  // lock P2
struct sembuf Player2_unlock = {semaphore_2, 1, 0}; // unlock P2

int playerMove(int player);
void clearMemory();
int isFullBoard();
void copyToMemory();
void copyFromMemory();
void drawBoard();
void checkBoard(int player);
void checkResult();
void printArray();

int main() {
    system("clear");
    printf("\t Tic Tac Toe \n");

    if ((memory = shmget(key, sizeof(int) * 10, IPC_CREAT | IPC_EXCL | 0777)) == -1) {
        if ((memory = shmget(key, sizeof(int) * 10, 0)) == -1) {
            printf("\nCannot connect with game: error with sared memory.\n");
            exit(1);
        } else {
            printf("\nYour name: ");
        }
    } else {
        printf("\nYour name: ");
    }

    semafors = semget(key, 2, 0777 | IPC_CREAT | IPC_EXCL);

    if (semafors == -1) {
        semafors = semget(key, 2, 0);
        playername = "Player2";
        printf("Player2 - 'O'\n");
    } else {
        playername = "Player1";
        printf("Player1 - 'X'\n");
    }

    shared_memory = shmat(memory, 0, 0);
    copyToMemory();

    if (strcmp(playername, "Player1") == 0) {
        semctl(semafors, semaphore_1, SETVAL, 0);
        semctl(semafors, semaphore_2, SETVAL, 1);
        if (playerMove(Player1) == 0) {
            system("clear");
            copyToMemory();
            drawBoard();

            while (true) {
                printf("\nWaiting for move Player2\n");
                semop(semafors, &Player2_lock,1);
                system("clear");

                copyFromMemory();
                drawBoard();
                checkBoard(Player2);
                checkResult();

                if (playerMove(Player1) == 0) {
                    system("clear");
                    checkBoard(Player1);
                    copyToMemory();
                    drawBoard();
                    checkResult();
                }

                semop(semafors, &Player2_unlock, 1);
            }
        }
    } else {
        printf("\nWaiting for move Player1 \n");
        while (true) {
            copyFromMemory();

            if (isFullBoard() == 0) {
                while (true) {
                    printf("\nWaiting for move Player1\n");
                    semop(semafors, &Player1_lock, 1);
                    system("clear");

                    copyFromMemory();
                    drawBoard();
                    checkBoard(Player1);
                    checkResult();

                    if (playerMove(Player2) == 0) {
                        system("clear");
                        checkBoard(Player2);
                        copyToMemory();
                        drawBoard();
                        checkResult();
                    }

                    semop(semafors, &Player1_unlock, 1);
                }
            }
        }
    }

   return 0;
}

int playerMove(int player) {
    int x, y;
    int status = 1; // 0 - ok, 1 - wrong

    while (status == 1) {
        printf("\nType field x y [1-3]: ");
        scanf("%d %d", &x, &y);

        if ((x > 0 && x < 4) && (y > 0 && y < 4)) {
            if (board[y - 1][x - 1] == 0) {
                board[y - 1][x - 1] = player;
                status = 0;
                return 0;
            } else {
                printf("\n\nThe field is blocked.\n");
                status = 1;
            }
        } else {
            printf("\n\nWrong field number.\n");
            status = 1;
        }
    }
}

void clearMemory() {
    semctl(semafors, 0, IPC_RMID, 0);
    shmdt(shared_memory);
    shmctl(memory, IPC_RMID, 0);
    exit(0);
}

int isFullBoard() {
    if (board[0][0] == 0 && board[1][0] == 0 && board[2][0] == 0
        && board[0][1] == 0 && board[1][1] == 0 && board[2][1] == 0
        && board[0][2] == 0 && board[1][2] == 0 && board[2][2] == 0) {
        return 1;
    } else return 0;
}

void copyToMemory() {
    int counter = 0;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            shared_memory[counter++] = board[i][j];
        }
    }
}

void copyFromMemory() {
    int counter = 0;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            board[i][j] = shared_memory[counter++];
        }
    }
}

void drawBoard() {
    int i, j;
    printf("\n");

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++) {
            if (board[i][j] == 0) printf("  ");
            else if (board[i][j] == 1) printf(" X"); // X - Player1
            else if (board[i][j] == 2) printf(" O"); // O - Player2

            if (j == 0 || j == 1) printf(" |");
        }

        if (i == 0 || i == 1) printf("\n-----------\n");
    }

    printf("\n\n");
}

void checkBoard(int player) {
    int i, j;

    // no more empty fields on the board
    if (board[0][0] != 0 && board[0][1] != 0 && board[0][2] != 0
        && board[1][0] != 0 && board[1][1] != 0 && board[1][2] != 0
        && board[2][0] != 0 && board[2][1] != 0 && board[2][2] != 0) result = 3;
    else {
        if      (board[0][0] == player && board[1][0] == player && board[2][0] == player) result = player; // vertical 1
        else if (board[0][1] == player && board[1][1] == player && board[2][1] == player) result = player; // vertical 2
        else if (board[0][2] == player && board[1][2] == player && board[2][2] == player) result = player; // vertical 3
        else if (board[0][0] == player && board[0][1] == player && board[0][2] == player) result = player; // horizontal 1
        else if (board[1][0] == player && board[1][1] == player && board[1][2] == player) result = player; // horizontal 2
        else if (board[2][0] == player && board[2][1] == player && board[2][2] == player) result = player; // horizontal 3
        else if (board[0][0] == player && board[1][1] == player && board[2][2] == player) result = player; // slope 1
        else if (board[0][2] == player && board[1][1] == player && board[2][0] == player) result = player; // slope 2
    }
}

void checkResult() {
    if ((result == 1 && strcmp(playername, "Player1") == 0)
        || (result == 2 && strcmp(playername, "Player2") == 0)) {
        printf("\n\n You win!\n\n");
        clearMemory();
    } else if ((result == 1 && strcmp(playername, "Player2") == 0)
        || (result == 2 && strcmp(playername, "Player1") == 0)) {
        printf("\n\n Youe lose!\n\n");
        clearMemory();
    } else if (result == 3){
        printf("\n\n Remis! \n\n");
        clearMemory();
    }
}

void printArray() {
    int i, j;
    for (i = 0; i < 4; i++) {
        printf("%d: \n", i);
        for (j = 0; j < 4; j++) {
            printf("%d \n", board[i][j]);
        }
    }
}
