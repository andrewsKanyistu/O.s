#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "mylib.h"

int main (void) {
	int fd;

	char buf1[] = "PRIMA STRINGA";
	char buf2[] = "SECONDA STRINGA";

	if((fd = creat("file.hole", 0666)) < 0)
		syserr("create_hole", "create file.hole error");
	                 // sizeof(buf1)
	if(write(fd, buf1, 13) != 13)		/* offset del file = 13*/
		syserr("create_hole","write buf1 error");

	/*
		Creo un buco all'interno del file forzando la lseek a
		spostare la posizione del puntatore oltre la fine
		del file
	*/
	if(lseek(fd, 40, SEEK_SET) == -1)		/* offset del file = 40*/
		syserr("create_hole", "lseek error");

	if(write(fd, buf2, 15) != 15)		/* offset del file = 55*/
		syserr("create_hole", "write buf2 error");

    return 0;
}
