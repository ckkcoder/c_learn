#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>	//	struct utmp
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	//	open() write() lseek() fctnl() etc.
#include <unistd.h>	//	close() read() 
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <ttyent.h>	//查找当前登录终端的信息

#define SHOWHOST
#define UTFISIZE 512


void usage();
int checkpra(int, char *);
void show_info(struct utmp*);
void show_time(int32_t *);
extern int utmp_open(char *);
extern struct utmp *utmp_next();
extern void utmp_close();

static char UTMP_FILE_S[ UTFISIZE ] = UTMP_FILE;

int main(int argc, char *argv[])
{
	int ami_flag = -1;
	struct utmp	*utbufp;
	char tty_name[15];
	
	if( argc == 2 ){
		strncpy(UTMP_FILE_S,argv[1],UTFISIZE);
	}else if( !((argc == 3 && checkpra(1,argv[1]) && checkpra(2,argv[2]) && (ami_flag=1) ) || argc == 1 )){
		usage();
	}

	if( utmp_open( UTMP_FILE_S ) == -1 ){
		perror( UTMP_FILE_S );
		exit(1);
	}
	while( (utbufp = utmp_next()) != ((struct utmp *)NULL) ){
		if( argc == 3 && ami_flag == 1 ){
			strcpy(tty_name, "/dev/");
			//printf("%s\n",ttyname(0));
			//printf("%s\n",utbufp->ut_line);
			strcat(tty_name,utbufp->ut_line);
			if( strcmp(tty_name,ttyname(0)) == 0 ){
				show_info(utbufp);
				break;
			}else{
				continue;
			}
		}else{
			show_info(utbufp);
		}
	}
	utmp_close();
	return 0;
}

void usage()
{
	fprintf(stderr,"Usage: who [am/Am/aM/AM i/I]\n");
	exit(1);
}

int checkpra(int argc_num, char *argv_one)
{
	int flag = 0;
	char spar[5];
	if( argc_num == 1 ){
		return (strcmp(argv_one,"am")==0 || \
				strcmp(argv_one,"Am")==0 || \
				strcmp(argv_one,"aM")==0 || \
				strcmp(argv_one,"AM")==0 );
	}else if( argc_num == 2 ){
		return (strcmp(argv_one,"i")==0 || \
				strcmp(argv_one,"I")==0 );
	}
	
	return 0;
}

void show_info(struct utmp *utbufp)
{
	if( utbufp->ut_type != USER_PROCESS )
		return;
	printf("%-8.8s",utbufp->ut_name);
	printf(" ");
	printf("%-8.8s",utbufp->ut_line);
	printf(" ");
	//printf("%10d",utbufp->ut_time);
	//if( kill(utbufp->ut_pid,0) != -1 ){
		show_time(&(utbufp->ut_time));
		printf(" ");
	//}
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
