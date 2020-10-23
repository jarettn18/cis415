#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	
	//Write files in directory to stdout
	while (read_dir) {
		char *name = read_dir->d_name;
		write(1, name, strlen(read_dir->d_name));
		write(1, " ", 1);
		read_dir = readdir(opened_dir);
	}
	write(1, "\n", 1);

	closedir(opened_dir);
	free(read_dir);
	free(buf);
}

void showCurrentDir() {
	char *buf = NULL;
	size_t size = 0;

	//Get current directory
	char *cwd = getcwd(buf,size);

	//write to stdout
	write(1, cwd, strlen(cwd));
	write(1, "\n", 1);

	free(buf);
}

void makeDir(char *dirName) {

	//Check if absolute path name
	char *dir_name_abs = strrchr(dirName, '/');
	if (dir_name_abs != NULL) {
		int made_dir_abs = mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH);
		if (made_dir_abs == 0) {
			return;
		}
		else {
			write(2, "ERROR: PATH NOT FOUND\n", 23);
			return;
		}
	}

	char *buf = NULL;
	size_t size = 0;
	char *dir_path;
	char *cwd = getcwd(buf, size);
	int made_dir;

	//Path name is relative append /*dir name* to path
	dir_path = strcat(cwd, "/");
	dir_path = strcat(cwd, dirName);

	//Make directory chmod 775
	made_dir = mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (made_dir != 0) {
		write(2, "ERROR: DIRECTORY NOT MADE\n", 26);
	}

	free(buf);
}

void changeDir(char *dirName) {

	//Check if absolut path name
	int change_dir_abs = chdir(dirName);
	if (change_dir_abs == 0) {
		return;
	}

	char *buf = NULL;
	size_t size = 0;

	//Not absolute path name
	char *cwd = getcwd(buf, size);
	char *dir_path = strcat(cwd, "/");
	dir_path = strcat(cwd, dirName);

	//Change directory
	int changed_dir = chdir(dir_path);
	if (changed_dir != 0) {
		write(2, "ERROR: DIRECTORY CHANGE FAILED\n", 31);
	}
	free(buf);
}

void displayFile(char *filename) {

	//Buf for CWD and file buffer for line
	char *buf = NULL;
	char *file_buf = malloc(sizeof(char) * 1000);
	size_t size = 0;

	//Get current directory
	char *dir_path;
	char *cwd = getcwd(buf,size);

	//Check if absolute path name
	char *dir_name_abs = strrchr(filename, '/');
	if (dir_name_abs != NULL) {
		dir_path = filename;
	}
	else {
		//Relative path name
		dir_path = strcat(cwd,"/");
		dir_path = strcat(cwd,filename);
	}

	//Open file
	int opened_file = open(dir_path, O_RDONLY);

	//File Not Valid
	if (opened_file == -1) {
		write(2, "ERROR: UNABLE TO OPEN FILE\n", 27);
	}
	else {
		//Write file to stdout
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
}

void deleteFile(char *filename) {
	char *buf = NULL;
	size_t size = 0;
	char *dir_path;
	char *cwd = getcwd(buf,size);

	//Check if absolute path name
	char *dir_name_abs = strrchr(filename, '/');
	if (dir_name_abs != NULL) {
		dir_path = filename;
	}
	else {
		//Relative path name
		dir_path = strcat(cwd,"/");
		dir_path = strcat(cwd,filename);
	}

	//Remove file
	int removed_file = unlink(dir_path);

	//Unsuccessful unlink
	if (removed_file != 0) {
		write(2, "ERROR: FILE NOT DELETED\n", 24);
	}
	free(buf);
}

void moveFile(char *sourcePath, char *destinationPath) {

	char *buf = NULL;
	size_t size = 0;

	char *cwd_s = getcwd(buf,size);
	char *cwd_d = getcwd(buf,size);

	//ABS PATH CHECK SOURCE
	char *source_p;

	char *dir_name_abs_S = strrchr(sourcePath, '/');
	if (dir_name_abs_S != NULL) {
		source_p = sourcePath;
	}
	else {
		source_p = strcat(cwd_s,"/");
		source_p = strcat(cwd_s,sourcePath);
	}

	//ABS PATH CHECK DEST
	char *dest_p;

	char *dir_name_abs_D = strrchr(destinationPath, '/');
	if (dir_name_abs_D != NULL) {
		dest_p = destinationPath;
	}
	else {
		dest_p = strcat(cwd_d,"/");
		dest_p = strcat(cwd_d,destinationPath);
	}

	//Rename files
	int renamed = rename(source_p, dest_p);

	if (renamed == 0) {
		return;
	}
	else {
		write(2, "ERROR: FILE CANNOT BE MOVED\n", 28);
	}
	free(buf);
}

void copyFile(char *sourcePath, char *destinationPath) {
	
	char *buf = NULL;
	char *file_buf = malloc(sizeof(char) * 1000);
	size_t size = 0;

	char *cwd_s = getcwd(buf,size);
	char *cwd_d = getcwd(buf,size);

	//ABS PATH CHECK SOURCE
	char *source_p;

	char *dir_name_abs_S = strrchr(sourcePath, '/');
	if (dir_name_abs_S != NULL) {
		source_p = sourcePath;
	}
	else {
		source_p = strcat(cwd_s,"/");
		source_p = strcat(cwd_s,sourcePath);
	}

	//ABS PATH CHECK DEST
	char *dest_p;
	char *dir_name_abs_D = strrchr(destinationPath, '/');

	if (dir_name_abs_D != NULL) {
		dest_p = destinationPath;
	}
	else {
		dest_p = strcat(cwd_d,"/");
		dest_p = strcat(cwd_d,destinationPath);
	}

	//COPY FILE TO DEST PATH
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
	//chmod(dest_p, S_IRUSR | S_IRGRP | S_IROTH);
	close(opened_file);
	close(copied_file);
	free(buf);
}
	


