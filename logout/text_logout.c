#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>
#include "utmplib.h"
#include <time.h>


#define SHOWHOST
#undef 	UTMP_FILE
#define UTMP_FILE "utmp"

void show_info(struct utmp*);
void show_time(int32_t *);

int main()
{
	struct utmp *utbufp;
	if( utmp_open( UTMP_FILE ) == -1 ){
		exit(1);
	}

	while( ( utbufp = utmp_next() ) != ((struct utmp *)NULL) ){
		//show_info(utbufp);
		if( strcmp(utbufp->ut_user,"kevin") == 0 ){
			strcpy(utbufp->ut_user,"ken");
			if( utmp_seek(-1,SEEK_CUR) == -1 ){
				fprintf(stderr,"error");
				exit(1);
			}else{
				utmp_write(utbufp);
				//break;
			}
		}
	}
	
	if( utmp_seek(0,SEEK_SET) == -1 ){
		fprintf(stderr,"error2");
		exit(1);
	}

	while( ( utbufp = utmp_next() ) != ((struct utmp *)NULL) ){
		show_info(utbufp);
	}

	utmp_close();
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



