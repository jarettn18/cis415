#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

int main(int argc, char *argv[]) {
	//INTERACTIVE MODE
	if (argc == 1) {
	}
	//FILE MODE	
	else if (argc == 3) {
		if (strncmp("-f",argv[1], 2) != 0) {
			fprintf(stderr, "ERROR: Unrecognized flag '%s'\n",argv[1]);
			fprintf(stderr, "Usage: ./pseudo-shell -f <filename>\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "ERROR: Too many arguments\n");
		fprintf(stderr, "Usage: ./pseudo-shell or ./pseudo-shell -f <filename>");
		exit(EXIT_FAILURE);
	}
}
