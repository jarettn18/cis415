#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "command.h"

void listDir() {

	//Buffer assigned to NULL which raises memory errors. See Notes.
	char *buf = NULL;
	size_t size = 0;

	//Open Current Directory
	char *cwd = getcwd(buf, size);
	DIR *opened_dir = opendir(cwd);
	struct dirent *read_dir = readdir(opened_dir);
	
	while (read_dir) {
		char *name = read_dir->d_name;
		write(1, name, strlen(read_dir->d_name));
		write(1, "\n", 1);
		read_dir = readdir(opened_dir);
	}

	closedir(opened_dir);
	free(read_dir);
	free(buf);
	free(cwd);
}

void showCurrentDir() {
	char *buf = NULL;
	size_t size = 0;

	char *cwd = getcwd(buf,size);
	write(1, cwd, strlen(cwd));
	write(1, "\n", 1);

	free(buf);
	free(cwd);
}

void makeDir(char *dirName) {
	char *buf = NULL;
	size_t size = 0;

	char *cwd = getcwd(buf, size);
	char *dir_path = strcat(cwd, "/");
	dir_path = strcat(cwd, dirName);
	int made_dir = mkdir(dir_path, 0);
	if (made_dir != 0) {
		write(2, "ERROR: DIRECTORY NOT MADE\n", 26);
	}

	free(buf);
	free(cwd);
}

void changeDir(char *dirName) {
	char *buf = NULL;
	size_t size = 0;

	char *cwd = getcwd(buf, size);
	char *dir_path = strcat(cwd, "/");
	dir_path = strcat(cwd, dirName);
	int changed_dir = chdir(dir_path);
	if (changed_dir != 0) {
		write(2, "ERROR: DIRECTORY CHANGE FAILED\n", 31);
	}
	free(buf);
	free(cwd);
}

void displayFile(char *filename) {
	
	char *buf = NULL;
	char *file_buf = malloc(sizeof(char) * 1000);
	size_t size = 0;

	char *cwd = getcwd(buf,size);
	char *dir_path = strcat(cwd,"/");
	dir_path = strcat(cwd,filename);

	int opened_file = open(dir_path, O_RDONLY);

	if (opened_file == -1) {
		write(2, "ERROR: UNABLE TO OPEN FILE\n", 27);
		return;
	}
	else {
		int bytes_read = 0;
		bytes_read = read(opened_file, file_buf, 1000);
		while (bytes_read != 0) {
			write(1, file_buf, bytes_read);
			bytes_read = read(opened_file, file_buf, 1000);
		}
	}
	free(file_buf);
	free(buf);
	free(cwd);
}

	


