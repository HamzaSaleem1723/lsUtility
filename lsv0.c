/*Receives exactly one directory name via command line
argument and display names of files and subdirectories in
the order as they appear in the directory*/

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;
void do_ls(char *);

int main(int argc, char* argv[]){

	if(argc != 2){
		printf("You can only pass one directory name\n");
		exit(0);
	}
	printf("Directory listing of %s:\n", argv[1] );
	do_ls(argv[1]);
	return 0;
}

void do_ls(char *dir){
	
	DIR* dp = opendir(dir);
	if(dp == NULL){
		fprintf(stderr,"Error in opening file\n");
	}
	errno=0;
	struct dirent * entry;

	while((entry = readdir(dp)) != NULL){
		if(entry == NULL && errno != 0){
			perror("Readdir Failure");
			exit(errno);
		
		}
		else{
			printf("%s\n",entry->d_name);
		
		}
		
	}
	closedir(dp);

}
