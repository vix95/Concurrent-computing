#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char home_dir[128] = "/home/vix/programowanie_wspolbiezne/Lab_02/zad_1/";

    // assing paths
    char REQUEST_BUFFER[128] = "";
    strcat(REQUEST_BUFFER, home_dir);
    strcat(REQUEST_BUFFER, "/wiadomosc");

    char RESPONSE_BUFFER[128] = "";
    strcat(RESPONSE_BUFFER, home_dir);
    strcat(RESPONSE_BUFFER, "/odpowiedz");

    char LOCKFILE[128] = "";
    strcat(LOCKFILE, home_dir);
    strcat(LOCKFILE, "/lockfile");

    // waiting for response
    while (1) {
        printf("Czekam na wiadomosc od konsumentow...\n");
        int i = 0;
        int c;
        int file;

        // waiting for the message file from user
        while ((file = open(REQUEST_BUFFER, O_RDONLY, S_IRWXU)) == -1) {}

        char message_from_user[1024];
        read(file, message_from_user, 1024);

        // read the message
        for (; (c = message_from_user[i]) != 27; i++) {
            putchar(c);
        }

        close(file);
        unlink(REQUEST_BUFFER); // delete buffer

        // type response to user
        printf("\nWpisz odpowiedz:\n");
        char response_to_user[1024];
        i = 0;

        do {
            c = getchar();
            response_to_user[i++] = (char)c;
        } while (c != 27);

        response_to_user[i] = '\0';

        // write response to response buffer
        file = open(RESPONSE_BUFFER,
                O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, S_IRWXU);

        if (file == -1) { perror("nie mozna otworzyc pliku"); }
        write(file, response_to_user, strlen(response_to_user));
        close(file);

        unlink(LOCKFILE); // release lock

        printf("\n");
    }

    return 0;
}
