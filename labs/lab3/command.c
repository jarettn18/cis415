#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "command.h"

void lfcat() {

	//Buffer assigned to NULL raises memory errors. See Notes.
	char *buf = NULL;
	char *line = NULL;
	size_t size = 0;

	//Open Current Directory
	char *cwd = getcwd(buf, size);
	DIR *opened_dir = opendir(cwd);
	struct dirent *read_dir = readdir(opened_dir);

	//Redirect STDOUT to output.txt
	freopen("output.txt", "w+", stdout);

	//While unread directories exist
	while (read_dir) {

		//Write directory name to file
		write(1, read_dir->d_name, strlen(read_dir->d_name));
		write(1, "\n" , 1);

		//Open file
		FILE *fp = fopen(read_dir->d_name, "r");

		//Write contents to STDOUT
		int line_len = 0;
		while (line_len != -1) { 
			line_len = getline(&line, &size, fp);
			write(1, line, line_len);
		}

		//Write 80 "-"
		write(1,"\n", 1);
		for (int i = 0 ; i < 80 ; i++) {
			write(1,"-", 1);
		}
		write(1,"\n", 1);

		//Close current file and read next file
		fclose(fp);
		read_dir = readdir(opened_dir);
	}

	closedir(opened_dir);
	free(read_dir);
	free(buf);
	free(line);
	free(cwd);
}
