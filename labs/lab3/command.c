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
	char *line = NULL;
	size_t size = 0;

	char *cwd = getcwd(buf, size);
	DIR *opened_dir = opendir(cwd);
	struct dirent *read_dir = readdir(opened_dir);

	freopen("output.txt", "w+", stdout);

	while (read_dir) {
		write(1, read_dir->d_name, strlen(read_dir->d_name));
		write(1, "\n" , 1);

		FILE *fp = fopen(read_dir->d_name, "r");
		int line_len = 0;
		while (line_len != -1) { 
			//TODO
			line_len = getline(&line, &size, fp);
			write(1, line, line_len);
		}

		read_dir = readdir(opened_dir);
	}

	closedir(opened_dir);
	free(cwd);
}
