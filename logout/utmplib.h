/*	utmplib.c - function tu buffer reads from utmp file
 *	utmplib.h	__UTMPLIB_H__
 *	@auther: kevin
 *	
 *	function are
 *  	utmp_open( filename )
 *  	utmp_next()
 *  	utmp_close()
 *  	utmp_seek()
 */

#ifndef __UTMPLIB_H__
#define __UTMPLIB_H__

extern int utmp_open(char *);
extern struct utmp *utmp_next();
extern int utmp_reload();
extern int utmp_seek();
extern int utmp_write(struct utmp *);
extern int utmp_close();

#endif	/* __UTMPLIB_H__ */
