/*This version adds a feature that does not display the
hidden files in the directory*/

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

	if(argc == 1){
		do_ls(".");
	}
	else{
		for(int i=1; i< argc; i++){
			printf("Directory listing of %s:\n", argv[i] );
			do_ls(argv[i]);
		}
	}
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
			if(entry->d_name[0] == '.')
				continue;
				
			printf("%s\n",entry->d_name);
		
		}
		
	}
	closedir(dp);

}
