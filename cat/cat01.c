#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define STRSIZE 2048

char strbuf[ STRSIZE + 1 ] = "\0";

void usage(char *s)
{
	fprintf(stderr,"usage: %s [file]",s);
	exit(1);
}

void oops(char *s1,char *s2)
{
	write(1,"cat: ",5);
	perror(s2);
}

int main(int argc,char *argv[])
{
	int rec_size = 0;
	int file_num;
	int fd;
	if( argc == 1 ){
		do{
			rec_size = read(0,strbuf,STRSIZE);
			strbuf[STRSIZE] = 0;
			write(1,strbuf,rec_size);
			fflush(stdin);
		}while(1);
		exit(0);
	}

	for( file_num=1; file_num < argc; file_num++ ){
		if( (fd = open(argv[file_num],O_RDONLY)) == -1 ){
			oops("",argv[file_num]);
			continue;
		}
		while( (rec_size = read(fd,strbuf,STRSIZE)) != -1 ){
			if( rec_size == 0 )
				break;
			if( write(1,strbuf,rec_size) != rec_size )
				oops("","Print the file error");
		}
		if( close(fd) == -1 ) oops("",argv[file_num]);
	}

	return 0;
}
