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

//TODO WRITE ABSOLUTE PATH FUNCTIONALITY FOR MV AND CP
// CHECK FOR MEMORY LEAKS AND ERRORS
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
	int change_dir_abs = chdir(dirName);
	if (change_dir_abs == 0) {
		return;
	}
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
	}
	else {
		int bytes_read = 0;
		bytes_read = read(opened_file, file_buf, 1000);
		while (bytes_read != 0) {
			write(1, file_buf, bytes_read);
			bytes_read = read(opened_file, file_buf, 1000);
		}
	}
	close(opened_file);
	free(file_buf);
	free(buf);
	free(cwd);
}

void deleteFile(char *filename) {
	printf("deleting\n");
	char *buf = NULL;
	size_t size = 0;

	char *cwd = getcwd(buf,size);
	char *dir_path = strcat(cwd,"/");
	dir_path = strcat(cwd, filename);

	int removed_file = remove(dir_path);
	if (removed_file != 0) {
		write(2, "ERROR: FILE NOT DELETED\n", 24);
	}
	free(cwd);
	free(buf);
}

void moveFile(char *sourcePath, char *destinationPath) {

	char *buf = NULL;
	size_t size = 0;

	char *cwd_s = getcwd(buf,size);
	char *cwd_d = getcwd(buf,size);

	char *source_p = strcat(cwd_s, "/");
	char *dest_p = strcat(cwd_d, "/");
	
	source_p = strcat(cwd_s, sourcePath);
	dest_p = strcat(cwd_d, destinationPath);

	int renamed_abs_abs = rename(sourcePath, destinationPath);
	int renamed_rel_abs = rename(source_p, destinationPath);
	int renamed_abs_rel = rename(sourcePath, dest_p);
	int renamed = rename(source_p, dest_p);

	if (renamed == 0) {
		return;
	}
	else if (renamed_abs_abs == 0) {
		return;
	}
	else if (renamed_rel_abs == 0) {
		return;
	}
	else if (renamed_abs_rel == 0) {
		return;
	}
	else {
		write(2, "ERROR: FILE CANNOT BE MOVED\n", 28);
	}
	free(buf);
	free(cwd_s);
	free(cwd_d);
}

void copyFile(char *sourcePath, char *destinationPath) {
	
	char *buf = NULL;
	char *file_buf = malloc(sizeof(char) * 1000);
	size_t size = 0;

	char *cwd_s = getcwd(buf,size);
	char *cwd_d = getcwd(buf,size);

	char *source_p = strcat(cwd_s,"/");
	source_p = strcat(cwd_s,sourcePath);

	char *dest_p = strcat(cwd_d,"/");
	dest_p = strcat(cwd_d,destinationPath);

	int opened_file = open(source_p, O_RDONLY);
	int copied_file = open(dest_p, O_RDWR | O_CREAT);

	if (opened_file == -1) {
		write(2, "ERROR: UNABLE TO OPEN FILE\n", 27);
	}
	else {
		int bytes_read = 0;
		bytes_read = read(opened_file, file_buf, 1000);
		while (bytes_read != 0) {
			write(copied_file, file_buf, bytes_read);
			bytes_read = read(opened_file, file_buf, 1000);
		}
	}
	close(opened_file);
	close(copied_file);
	free(buf);
	free(cwd_s);
	free(cwd_d);
}
	


