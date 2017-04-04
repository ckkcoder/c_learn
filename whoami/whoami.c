#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
	struct passwd *pwdbufp;
	
	pwdbufp = getpwuid( geteuid() );
	
	if( pwdbufp == NULL ){
		perror(*argv);
		return -1;
	}	
	printf("%s\n",pwdbufp->pw_name);
	return 0;
}
