#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE	4096
#define COPYMODE	0644

void oops(char *,char *);

int main(int argc,char *argv[])
{
	int fd_r,fd_w,readsize = 0;
	char buf[BUFFERSIZE];
	if( argc != 3 ){
		fprintf(stderr,"usage: %s source destination\n",*argv);
		exit(1);
	}

	if( (fd_r = open(argv[1],O_RDONLY)) == -1 ){
		oops("Cannot open",argv[1]);
	}
	if( (fd_w = creat(argv[2],COPYMODE)) == -1 ){
		oops("Cannot creat",argv[2]);
	}

	while( (readsize = read(fd_r,buf,BUFFERSIZE)) > 0 ){
		if( write(fd_w,buf,readsize) != readsize ){
			oops("Write error to",argv[2]);
		}
	}

	if( readsize == -1 ){
		oops("Read error from",argv[1]);
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
