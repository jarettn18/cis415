#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "command.h"

void lfcat() {
	printf("<<In lfcat(): Step-01: Function called>>\n");
	printf("<<In lfcat(): Step-02: Lising all files in current dir>>\n");
	char *buf = NULL;
	size_t size = 0;

	getcwd(buf, size);
	while(*buf) {
		printf("%c",*buf);
		buf++;
	}

}
