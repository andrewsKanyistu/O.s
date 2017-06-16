#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#define READ 0
#define WRITE 1
#ifndef SIZE
#define SIZE 4096   // dimensione in bytes da leggere da file
#endif

typedef struct {
	char operazione;    		//messaggio da mandare ai figli
	int riga;
	int colonna;
	int dimensione;
	//int *ptr_matriceA;
	//int *ptr_matriceB;
	//int *ptr_matriceC;
}msg;
typedef struct {
	long mtype;
	char text[SIZE];
}msg_queue;

int main(int argc, char *argv[]){
int fd_matriceA;
int fd_matriceB;
int fd_matriceC;
int i=0,j=0,k=0;											// variabili contatore
char *token;
struct shmid_ds sharedmemory;
msg_queue toParent;
char bufferA[SIZE];
char bufferB[SIZE];
char bufferC[SIZE];
int shmid_A=0,shmid_B=0,shmid_C=0,shmid_sum;		// id per modificare la memoria condivisa
const int dim=atoi(argv[3]); 					// dimensione delle matrici

//matrici dove verranno salvati i dati letti da disco
int matriceA[dim][dim];
int matriceB[dim][dim];
int matriceC[dim][dim];

// generazione della chiave per la memoria condivisa A;B;C e per la somma
key_t shm_key,shm_keyB,shm_keyC,shm_sum;
if((shm_key=ftok(argv[0],'a'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyB=ftok(argv[0],'b'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyC=ftok(argv[0],'c'))==-1){
	perror("Error nella creazione della chiave matrice c\n");
}
if((shm_sum=ftok(argv[0],'k'))==-1){
	perror("Error nella creazione della chiave somma c\n");
}


//Verifico se i parametri inseriti sono corretti
if(argc < 5){
	printf(" Numero di argomenti insufficienti o non corretti\n" );
	exit(1);
}

const int nProc=atoi(argv[4]);



// apro i files
if(!(fd_matriceA=open(argv[1],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

if(!(fd_matriceB=open(argv[2],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

if(!(fd_matriceC=open(argv[3],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

// leggo i file e li carico in buffer
if(!read(fd_matriceA,bufferA,SIZE))
	printf("file A  non letto correttamente\n");

if(!read(fd_matriceB,bufferB,SIZE))
	printf("file B non letto correttamente\n");


// NB la matrice C non deve essere VUOTA
if(!read(fd_matriceC,bufferC,SIZE)){
	printf("file C  non letto correttamente\n");
}

//==========PARSE MATRICE A===================
token=strtok(bufferA,";\n");		// tokenizzo la stringa con spazio e a capo
i =0, j=0;
while(token != NULL){
	matriceA[i][j]=atoi(token);
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}

//========== PARSE MATRICE B============

token=strtok(bufferB,";\n");		// tokenizzo la stringa con spazio e a capo
i =0, j=0;
while(token != NULL){
	matriceB[i][j]=atoi(token);
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}


// == sezione shared memory=====

if((shmid_A=shmget(shm_key,sizeof(matriceA),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on A\n");

if((shmid_B=shmget(shm_keyB,sizeof(matriceB),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on B\n");

if((shmid_C=shmget(shm_keyC,sizeof(matriceC),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on C\n");

if((shmid_sum=shmget(shm_sum,sizeof(int),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on sum\n");

int (*attach_A)[dim];
int (*attach_B)[dim];
int (*attach_C)[dim];
int *shm_sumValue;
attach_A= shmat(shmid_A,NULL,0);
attach_B=shmat(shmid_B,NULL,0);
attach_C=shmat(shmid_C,NULL,0);
shm_sumValue= (int *)shmat(shmid_sum,NULL,0);
int queue;
if((queue=msgget(IPC_PRIVATE,(IPC_CREAT|0660))==-1))
	printf("Failed Creation message queue\n" );

printf("dimensione matrici %i\n",dim );
// copio le matrici A e B in memoria condivisa
for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){

		attach_A[i][j]=matriceA[i][j];
		attach_B[i][j]=matriceB[i][j];
		attach_C[i][j]=0;
		//printf("%i\n", attach_A[i][j]);
	}
}
// stampo le matrici a video
for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){
		if(j==dim-1){
			printf("\n" );
		}
		printf("%i\t", matriceA[i][j]);

	}
}

srand(time(NULL));
 *shm_sumValue=100;
 int temp=*shm_sumValue;
 temp++;
 *shm_sumValue=temp;

pid_t children;
int pipes[nProc-1][2];
printf("\nNumero processi==>%i\n",nProc);
// read==>0 write===>1

for (i=0;i<nProc;i++){
	pipe(pipes[i]);
	children=fork();
	if(children==0){
		// codice del figlio
		msg msg_parent;
		close(pipes[i][WRITE]);

			//printf("figlio %i\n",i );
			// chiudo la pipe in scrittura
			int status;

			while(1){
				if ((status =read(pipes[i][READ],&msg_parent,sizeof(msg))==-1)) {
					perror("read failed on pipe");
				};

				switch (msg_parent.operazione) {
					case 'k':{
						//printf("asfdifhbud ==>%c figlio %i\n", msg_parent.operazione,i);
						printf("figlio %i terminato\n",i );
						exit(0);
						break;
					}
					case 'm':{
						int cRiga=msg_parent.riga;
						int cCol=msg_parent.colonna;

						//printf("Operazione ==>%c figlio %i\n", msg_parent.operazione,i);
						//printf("riga==> %i, colonna ==> %i, \n",msg_parent.riga,msg_parent.colonna);
						//int matRiga[dim];
						int num=0;

						for(j=0;j<dim;j++){
							attach_C[cRiga][cCol]+=attach_A[cRiga][j]*attach_B[j][cCol];
							num=attach_C[cRiga][cCol];

						}

						// dire al padre che ho finito
						//printf("nel figlio%i num ==>%i\n",i,attach_C[cRiga][cCol] );
						toParent.mtype=1;
						sprintf(toParent.text,"Figlio numero %i finito MOLTIPLICAZIONE con risultato%i",i,num);
						sleep(rand()%3);
						if(msgsnd(queue,&toParent,sizeof(toParent)-sizeof(long),0)==-1){
								perror("errore sul invio a padre");
						}

						break;
					}
				}

		}
	}

}

if(children >0){
	int n=0;
	//int riga=0,colonna=0;

msg msg_to_child;
//msg_queue fromChild;
//for(i=0;i<dim;i++)
	//printf("attach_A==> %i\n",attach_B[i][i] );


// chiudo i pipes alla in lettura
/*
for ( i = 0; i < nProc-1; i++) {

}*/
// MOLTIPLICAZIONE

for (i=0;i<dim;i++){
	close(pipes[i][READ]);
	for ( j = 0,n=0; j < dim; j++) {
		msg_to_child.operazione='m';
		msg_to_child.riga=i;
		msg_to_child.colonna=j;

		write(pipes[n][WRITE],&msg_to_child,sizeof(msg_to_child));
		if(n==nProc-1){
			n=0;
		}
		n++;
		if(msgrcv(queue,&toParent,sizeof(toParent),0,0)==-1){
			perror("Error message queue");
			exit(1);
		}
		printf("PADRE DICE:%s\n",toParent.text );
	}
}

//================== somma=====================


//sleep(5);

shmctl(shmid_sum,IPC_RMID,&sharedmemory);

	//for ( i = 0; i < nProc-1; i++) {
	//	wait(NULL); // aspetto la terminazione dei figli
	//}
	for ( i = 0; i < dim; i++) {
		for(k=0;k<dim;k++){
			printf("matrice C ==>%i\n",attach_C[i][k] );

		}
	}

	for ( i = 0,n=0; i < nProc; i++) {
			msg_to_child.operazione='k';
			write(pipes[i][WRITE],&msg_to_child,sizeof(msg_to_child));
	}
}






	//sleep(2);
	//wait(NULL);



printf("	==========%i=============\n",*shm_sumValue );


// elimino la memoria condivisa;

shmctl(shmid_A,IPC_RMID,&sharedmemory);
shmctl(shmid_B,IPC_RMID,&sharedmemory);
shmctl(shmid_C,IPC_RMID,&sharedmemory);

shmdt(NULL);

printf("ho finito \n" );
}
