#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char home_dir[128] = "/home/vix/zad2/";

    // read username from arguments
    if (argc < 2) {
        printf("Podaj nazwe uzytkownika jako argument\n");
        return 0;
    }

    char username[128] = "";
    strcat(username, argv[1]);

    // assign paths
    char request_buffer_filename[128] = "";
    strcat(request_buffer_filename, home_dir);
    strcat(request_buffer_filename, "/dane");

    char response_buffer_filename[128] = "";
    strcat(response_buffer_filename, home_dir);
    strcat(response_buffer_filename, "/wyniki");

    char lockfile_name[128] = "";
    strcat(lockfile_name, home_dir);
    strcat(lockfile_name, "/temp/lockfile");

    // wait if server is busy
    while (open(lockfile_name, O_CREAT | O_EXCL, 0) == -1) {
        printf("Serwer zajety, prosze czekac.\n");
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

    write(file, username, strlen(username)); // write username
    write(file, "\n", 1);
    write(file, message, buffer_length); // write message
    close(file);

    // wait for server response
    printf("\nProsze czekac na odpowiedz serwera...\n");
    while ((file = open(response_buffer_filename, O_RDONLY, S_IRWXU)) == -1) {}

    // read response from buffer
    char response_from_server[1024];
    read(file, response_from_server, 1024);
    int i;
    for (i = 0; (c = response_from_server[i]) != 27; i++) {}
    response_from_server[i] = '\0';

    // display response
    printf("Serwer odpowiedzial:\n%s\n", response_from_server);
    close(file);

    unlink(response_buffer_filename); // delete buffer
    return 0;
}
