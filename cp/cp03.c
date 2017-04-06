#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define BUFFERSIZE	4096
#define FILESIZE	512
#define COPYMODE	0644

void oops(char *,char *);
int check_no_same(struct stat *,struct stat *);

int main(int argc,char *argv[])
{
	int file_exist = -1;
	int ar_i,option_flag = 0;
	int fd_r,fd_w,readsize = 0,sou_f = -1,des_f = -1;
	char buf[BUFFERSIZE],soufile[FILESIZE],desfile[FILESIZE];
	struct stat stbuf1,stbuf2;
	if( argc != 3 && argc != 4 ){
		fprintf(stderr,"usage: %s [-i] source destination\n",*argv);
		exit(1);
	}
	
	for( ar_i=1; ar_i < argc; ar_i++ ){
		if( argv[ar_i][0] == '-' ){
			if( argv[ar_i][1] == 'i' ){
				option_flag = 1;
			}else{
				fprintf(stderr,"usage: %s [-i] source destination\n",*argv);
				exit(1);
			}
		}else{
			if( sou_f == -1 ){
				strncpy(soufile,argv[ar_i],BUFFERSIZE);
				sou_f = 1;
			}else{
				strncpy(desfile,argv[ar_i],BUFFERSIZE);
				des_f = 1;
			}
		}
	}

	if( sou_f == -1 || des_f == -1 ){
		fprintf(stderr,"usage: %s [-i] source destination\n",*argv);
		exit(1);
	}


	if( stat(soufile,&stbuf1) == -1 ){
		oops("stat",soufile);
	}

	if( stat(desfile,&stbuf2) == -1 ){
		//oops("stat",argv[2]);
		if( errno == ENOENT ){
			file_exist = 1;
		}
	}

	if( file_exist == -1 && option_flag == 1 ){
		char choice;
		fprintf(stdout,"cp: The %s file is exist,overwrite file? ",desfile);
		scanf("%c",&choice);
		if( !(choice == 'y' || choice == 'Y') ){
			fprintf(stderr,"cp: copy failed\n");
			exit(1);
		}
	}

	if( S_ISDIR(stbuf1.st_mode) ){
		fprintf(stderr,"cp: %s is a directory\n",soufile);
		exit(1);
	}

	if( S_ISDIR(stbuf2.st_mode) ){
		fprintf(stderr,"cp: %s is a directory\n",desfile);
		exit(1);
	}

	if( check_no_same(&stbuf1,&stbuf2) == 0 ){
		fprintf(stderr,"cp: Two documents for the same file\n");
		exit(1);
	}

	if( (fd_r = open(soufile,O_RDONLY)) == -1 ){
		oops("Cannot open",soufile);
	}
	if( (fd_w = creat(desfile,COPYMODE)) == -1 ){
		oops("Cannot creat",desfile);
	}

	while( (readsize = read(fd_r,buf,BUFFERSIZE)) > 0 ){
		if( write(fd_w,buf,readsize) != readsize ){
			oops("Write error to",desfile);
		}
	}

	if( readsize == -1 ){
		oops("Read error from",soufile);
	}

	if( close(fd_r) == -1 || close(fd_w) == -1 ){
		oops("Error closing files","");
	}

	return 0;
}

void oops(char *s1,char *s2)
{
	fprintf(stderr,"Error: %s ",s1);
	perror(s2);
	exit(1);
}

int check_no_same(struct stat *sp1,struct stat *sp2)
{
	return( !(sp1->st_ino == sp2->st_ino) );
}
