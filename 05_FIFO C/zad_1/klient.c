#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <unistd.h>

#define SERVERFIFO "/home/vix/programowanie_wspolbiezne/Lab_05/zad_1/serwerfifo"

int main(int argc, char *argv[]) {
	char *HOME = getenv("HOME");
	char message[128] = "\0";
	char readbuffer[128] = "\0";
	int sfifo = open(SERVERFIFO, O_WRONLY);

	if (argc != 2) {
		printf("%s\n", "ERROR: provide exactly 1 argument");
		printf("%s\n", "try: ./klient.out [ID]");
		return 0;
	} else if (sfifo == -1) {
		printf("%s\n", "ERROR: can't open the server fifo file");
		return 0;
	} else {
		char statement[128] = "\0";
		strcat(statement, argv[1]);
		strcat(statement, " ");
		strcat(statement, HOME);

		// get length of statement
		int i = 0;
		while (statement[i++] != '\0');

		// write the message
		sprintf(message, "%d %s", --i, statement);
		write(sfifo, message, strlen(message) + 1);
		close(sfifo);

		// set the path to clientfifo file and read the message
		char CLIENTFIFO[128];
		strcpy(CLIENTFIFO, HOME);
		strcat(CLIENTFIFO, "/klientfifo");
		FILE *f;
		f = fopen(CLIENTFIFO, "r");
		fgets(readbuffer, 128, f);
		printf("%s\n", readbuffer);
		fclose(f);
	}

	return 0;
}
