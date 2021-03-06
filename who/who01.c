#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>	//	struct utmp
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	//	open() write() lseek() fctnl() etc.
#include <unistd.h>	//	close() read() 
#include <time.h>
#include <sys/types.h>
#include <signal.h>

#define SHOWHOST

void show_info(struct utmp*);
void show_time(int32_t *);

int main()
{
	struct utmp	current_record;
	int 		utmpfd;
	int 		reclen = sizeof(current_record);

	if( (utmpfd = open(UTMP_FILE,O_RDONLY)) == EOF ){
		perror( UTMP_FILE );
		exit(1);
	}
	while( read(utmpfd,&current_record,reclen) == reclen ){
			show_info(&current_record);
	}
	close(utmpfd);
	return 0;
}

void show_info(struct utmp *utbufp)
{
	if( utbufp->ut_type != USER_PROCESS || kill(utbufp->ut_pid,0) == -1 )
		return;
	printf("%-8.8s",utbufp->ut_name);
	printf(" ");
	printf("%-8.8s",utbufp->ut_line);
	printf(" ");
	//printf("%10d",utbufp->ut_time);
	if( kill(utbufp->ut_pid,0) != -1 ){
		show_time(&(utbufp->ut_time));
		printf(" ");
	}
#ifdef SHOWHOST
	if( utbufp->ut_host[0] != '\0' )
		printf("(%s)",utbufp->ut_host);
#endif
	printf("\n");
}

void show_time(int32_t *tp)
{
	struct tm *tip;
	time_t time_tmp = (time_t)(*tp);
	//printf("%s",ctime(&time_tmp));
	//tip = gmtime(&time_tmp);
	tip = localtime(&time_tmp);
	printf("%8d-%02d-%02d %02d:%02d",tip->tm_year+1900,tip->tm_mon+1,tip->tm_mday,tip->tm_hour,tip->tm_min);
}
