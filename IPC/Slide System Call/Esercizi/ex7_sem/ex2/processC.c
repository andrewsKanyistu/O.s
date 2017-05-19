#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 

#define KEY 74

/* Procedere come per il processo B 
   Ricordare che il processo C ottiene ciclicamente la
	 risorsa dopo B e prima di A
*/
