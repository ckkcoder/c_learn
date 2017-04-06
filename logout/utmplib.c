/*	utmplib.c - function to buffer reads from utmp file
 *	
 *	function are
 *		utmp_open( filename )
 *		utmp_next()
 *		utmp_close()
 *		utmp_seek()
 *		utmp_write()
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <utmp.h>
#include "utmplib.h"

#define NRECS	16
#define UTSIZE	(sizeof(struct utmp))
#define NULLUT	((struct utmp *)NULL)

static char utmpbuf[ NRECS *UTSIZE ];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open( char *filename )
{
	fd_utmp = open( filename,O_RDWR );
	cur_rec = num_recs = 0;	
	return fd_utmp;
}

struct utmp *utmp_next()
{
	struct utmp *recp;
	if( fd_utmp == -1 )
		return NULLUT;
	if( cur_rec == num_recs && utmp_reload() == 0 )
		return NULLUT;

	recp = (struct utmp *)&utmpbuf[ cur_rec * UTSIZE ];
	cur_rec++;
	return recp;
}

int utmp_reload()
{
	int amt_read;
	amt_read = read( fd_utmp, utmpbuf, UTSIZE * NRECS );
	num_recs = amt_read / UTSIZE;
	cur_rec = 0;
	return num_recs;
}

int utmp_close()
{
	if( fd_utmp != -1 )
		return close(fd_utmp);
	return -1;
}

int utmp_seek(int record_offset, int base)
{
	int offset = record_offset * UTSIZE;
	if( base == SEEK_CUR ){
		offset = (record_offset + (cur_rec - num_recs)) * UTSIZE;
	}
	
	cur_rec = num_recs = 0;

	return lseek(fd_utmp,offset,base);
}

int utmp_write(struct utmp *utbufp)
{
	return write(fd_utmp,utbufp,UTSIZE);
}

