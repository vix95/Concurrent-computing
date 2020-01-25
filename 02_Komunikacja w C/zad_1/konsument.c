#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char home_dir[128] = "/home/vix/programowanie_wspolbiezne/Lab_02/zad_1/";

    // assign paths
    char request_buffer_filename[128] = "";
    strcat(request_buffer_filename, home_dir);
    strcat(request_buffer_filename, "/wiadomosc");

    char response_buffer_filename[128] = "";
    strcat(response_buffer_filename, home_dir);
    strcat(response_buffer_filename, "/odpowiedz");

    char lockfile_name[128] = "";
    strcat(lockfile_name, home_dir);
    strcat(lockfile_name, "/lockfile");

    while(1) {
        // wait if producent is busy
        while (open(lockfile_name, O_CREAT | O_EXCL, 0) == -1) {
            printf("Prodcuent jest zajety, prosze czekac.\n");
            sleep(1);
        }

        // type message
        printf("Wpisz wiadomosc:\n");
        char message[1024];
        int buffer_length = 0;
        int c;
        do {
            c = getchar();
            message[buffer_length++] = (char) c;
        } while (c != 27);

        message[buffer_length] = '\0';

        // write message to buffer
        int file;
        file = open(request_buffer_filename,
          O_WRONLY | O_CREAT | O_EXCL | O_APPEND, S_IRWXU);

        if (file == -1) printf("nie mozna otworzyc pliku\n");

        write(file, message, buffer_length); // write message
        close(file);

        // wait for producent response
        printf("\nProsze czekac na odpowiedz producenta...\n");
        while ((file = open(response_buffer_filename, O_RDONLY, S_IRWXU)) == -1) {}

        // read response from buffer
        char response_from_server[1024];
        read(file, response_from_server, 1024);
        int i;
        for (i = 0; (c = response_from_server[i]) != 27; i++) {}
        response_from_server[i] = '\0';

        // display response
        printf("Producent odpowiedzial:\n%s\n", response_from_server);
        close(file);

        unlink(response_buffer_filename); // delete buffer
    }
    return 0;
}
