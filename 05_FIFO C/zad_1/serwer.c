#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVERFIFO "/home/vix/programowanie_wspolbiezne/Lab_05/zad_1/serwerfifo"
#define ARRAY_SIZE 20
#define FIRST_ID 10

typedef int bool;
#define true 1
#define false 0

struct user {
	int ID;
	char *nazwisko;
};

int main() {
	struct user array[ARRAY_SIZE];
	array[0].nazwisko = "surname-0";
	array[1].nazwisko = "surname-1";
	array[2].nazwisko = "surname-2";
	array[3].nazwisko = "surname-3";
	array[4].nazwisko = "surname-4";
	array[5].nazwisko = "surname-5";
	array[6].nazwisko = "surname-6";
	array[7].nazwisko = "surname-7";
	array[8].nazwisko = "surname-8";
	array[9].nazwisko = "surname-9";
	array[10].nazwisko = "surname-10";
	array[11].nazwisko = "surname-11";
	array[12].nazwisko = "surname-12";
	array[13].nazwisko = "surname-13";
	array[14].nazwisko = "surname-14";
	array[15].nazwisko = "surname-15";
	array[16].nazwisko = "surname-16";
	array[17].nazwisko = "surname-17";
	array[18].nazwisko = "surname-18";
	array[19].nazwisko = "surname-19";

	int i = 0;
	for(i = 0; i < 20; i++) array[i].ID = i + FIRST_ID;

	while (true) {
		char readbuffer[128] = "\0"; // input from SERVERFIFO
		char buffer_length[3] = "\0"; // length of readbuffer
		char db_index[3] = "\0"; // index for struct database from readbuffer
		char client_home[40] = "\0";
		char answer[20] = "\0";
		bool found = false;

		// open server fifo and get message
		FILE *f;
		f = fopen(SERVERFIFO, "r");
		fgets(readbuffer, 128, f);
		fclose(f);
		//printf("Statement: %s\n", readbuffer);

		int pos = 0; // pos from readbuffer

		// get length of readbuffer
		int buffer_length_int = 0;
		if (readbuffer[pos] != 32) buffer_length[0] = readbuffer[pos++];
		if (readbuffer[pos] != 32) buffer_length[1] = readbuffer[pos++];
		if (readbuffer[pos] != 32) buffer_length[2] = readbuffer[pos++];
    	sscanf(buffer_length, "%d", &buffer_length_int);
		//printf("Length of statements is: %d\n", buffer_length_int);

		pos++; // pass space

		// get index of struct database
		int db_index_int = 0;
		if (readbuffer[pos] != 32) db_index[0] = readbuffer[pos++];
		if (readbuffer[pos] != 32) db_index[1] = readbuffer[pos++];
		if (readbuffer[pos] != 32) db_index[2] = readbuffer[pos++];
    	sscanf(db_index, "%d", &db_index_int);
		//printf("Loaded database index: %d\n", db_index_int);

		pos++; // pass space

		// get HOME path
		buffer_length_int += 1; // one for space
		if (db_index[0] != '\0') buffer_length_int += 3;
		if (db_index[1] != '\0') buffer_length_int += 2;
		if (db_index[2] != '\0') buffer_length_int += 1;
		int j = 0;
		while (pos < buffer_length_int) client_home[j++] = readbuffer[pos++];
		//printf("Loaded client HOME: %s\n", client_home);

		if (db_index_int >= FIRST_ID && db_index_int < FIRST_ID + 20) {
			i = 0;
			while (i < ARRAY_SIZE) {
				if (array[i].ID == db_index_int) {
					sscanf(array[i].nazwisko, "%s", answer);
					break;
				}
				i++;
			}
		} else {
			strcpy(answer, "Nie ma");
		}

		//printf("Answer is: %s\n", answer);

		// prepare path to client home dir
		strcat(client_home, "/klientfifo");
		//printf("The path to fifo file is: %s\n", client_home);

		int file;
		file = open(client_home, O_WRONLY);
    	if (file == -1) printf("Can't open the clientfifo file\n");
		else {
	    	if (write(file, answer, strlen(answer) + 1))
	    	close(file);
			printf("Saved message into clientfifo file\n");
		}
	}

	return 0;
}
