/*This version adds a feature that will display every file in the directory in long listing*/


#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


extern int errno;
void do_ls(char *);
void show_stat_info(char*, char *);
void do_ls_l(char *dir);

int main(int argc, char* argv[]){

	if(argc == 1){
		do_ls(".");
	}
	
	else if(strcmp(argv[1], "-l") == 0){
		for(int i=2; i< argc; i++){
			do_ls_l(argv[i]);
		}
		
	}
	
	else{
		for(int i=1; i<argc; i++){
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

void do_ls_l(char *dir){

	struct stat info;
	int rv = lstat(dir, &info);
	if (rv == -1){
	      perror("stat failed");
	      exit(1);
	}
	
	if ((info.st_mode &  0170000) == 0040000) {
	
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
			char buffer[1024];
			strcpy(buffer, dir);
			strcat(buffer, "//");
			strcat(buffer, entry->d_name);		
			show_stat_info(buffer,entry->d_name);		
		}
		
	}
	closedir(dp);
		
		
	
	}
		
	else if ((info.st_mode &  0170000) == 0100000) 
		show_stat_info(dir,dir);
   		
}



void show_stat_info(char *fname,char *filename){
   struct stat info;
   int rv = lstat(fname, &info);
   if (rv == -1){
      perror("stat failed");
      exit(1);
   }
   
   struct passwd * pwd = getpwuid(info.st_uid);
  
   if (pwd == NULL){
      if (errno == 0)
         printf("Record not found in passwd file.\n");
      else{
         perror("getpwuid failed");
         exit(errno);
      }
   }
   
   struct group * grp = getgrgid(info.st_gid);
   
   if (grp == NULL){
      if (errno == 0)
          printf("Record not found in /etc/group file.\n");
      else{
          perror("getgrgid failed");
          exit(errno);
      }
   }
   
   long secs = info.st_mtime;
   
   int mode = info.st_mode; 
   char str[10];
   strcpy(str, "---------");
//owner  permissions
   if((mode & 0000400) == 0000400) str[0] = 'r';
   if((mode & 0000200) == 0000200) str[1] = 'w';
   if((mode & 0000100) == 0000100) str[2] = 'x';
//group permissions
   if((mode & 0000040) == 0000040) str[3] = 'r';
   if((mode & 0000020) == 0000020) str[4] = 'w';
   if((mode & 0000010) == 0000010) str[5] = 'x';
//others  permissions
   if((mode & 0000004) == 0000004) str[6] = 'r';
   if((mode & 0000002) == 0000002) str[7] = 'w';
   if((mode & 0000001) == 0000001) str[8] = 'x';
//special  permissions
   if((mode & 0004000) == 0004000) str[2] = 's';
   if((mode & 0002000) == 0002000) str[5] = 's';
   if((mode & 0001000) == 0001000) str[8] = 't';
   

   if ((info.st_mode &  0170000) == 0010000) 
		printf("p");
   else if ((info.st_mode &  0170000) == 0020000) 
		printf("c");
   else if ((info.st_mode &  0170000) == 0040000) 
		printf("d");
   else if ((info.st_mode &  0170000) == 0060000) 
		printf("b");
   else if ((info.st_mode &  0170000) == 0100000) 
		printf("-");
   else if ((info.st_mode &  0170000) == 0120000) 
		printf("l");
   else if ((info.st_mode &  0170000) == 0140000) 
		printf("s");
   
   printf("%s ", str);
   printf("%ld ", info.st_nlink);
   printf("%s ", pwd->pw_name);
   printf("%s ", grp->gr_name);
   printf("%ld ", info.st_size);
   char *ctime_no_newline;
   ctime_no_newline = strtok(ctime(&secs), "\n");
   printf("%s ", ctime_no_newline);
   printf("%s\n", filename );
  
   
}



