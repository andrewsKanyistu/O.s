#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

#include <sys/wait.h>
#include <unistd.h>

#include <time.h>
#include <pthread.h>
#include <sys/msg.h>
#define PADRE 0		// identificativo del padre nel vettore di semafori
#define FIGLIO 1	// identificativo del un qualunque figlio nel vettore di semafori
#define READ 0
#define WRITE 1
#ifndef SIZE
#define SIZE 4096   // dimensione in bytes da leggere da file
#endif
int semid;

typedef struct {
 char operazione;    		//messaggio da mandare ai  thread figli
 int riga;
 int colonna;
int **matA,**matB,**matC;
int *somma;
int dimensione;
}msg;

pthread_mutex_t mutex;        // mutex per la sincronizzazione dei figli

void * threadFiglio (void * args){

	 msg* data=(msg * )args;
   int riga=data->riga;
   int colonna = data->colonna;
   char operazione = data->operazione;
   int **matriceC=data->matC;
   int **matriceA=data->matA;
   int **matriceB=data->matB;
   int dimensione=data->dimensione;
   printf("riga=> %i\t colonna =>%i\t operazione==>%c\t\n",riga,colonna,data->operazione);
   int i=0;

   switch(operazione){
     case 'm':{

      // sleep(1);
       for(i=0;i < dimensione ;i++){
         matriceC[riga][colonna]+=(matriceA[riga][i]*matriceB[i][colonna]);
         printf("matc==>%i,\triga=>%i,\t colonna->%i\n",matriceC[riga][colonna],riga,colonna );
       }

       break;
     }
     case 's':{
       //  faccio la somma della riga
       int temp=0;
       for (i=0;i<dimensione;i++){
         temp+=matriceC[i][0];
       }
       pthread_mutex_lock(&mutex);
       //  printf("ricevuto somma da padre\n" );
       sleep(1);
       *data->somma+=temp;
       pthread_mutex_unlock(&mutex);
       break;
     }
   }

  free(data);

	 return NULL;
}




int main(int argc, char *argv[]){
int fd_matriceA;
int fd_matriceB;
int fd_matriceC;
int i=0,j=0;											// variabili contatore
char *token;




char bufferA[SIZE];
char bufferB[SIZE];
char bufferC[SIZE];

 const int dim=atoi(argv[4]); 					// dimensione delle matrici

//matrici dove verranno salvati i dati letti da disco
int matriceA[dim][dim];
int matriceB[dim][dim];
int matriceC[dim][dim];

//Verifico se i parametri inseriti sono corretti
if(argc < 5){
	printf(" Numero di argomenti insufficienti o non corretti\n" );
	exit(1);
}


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


//allocazione di memoria per il valore che terra la somma e i vari vettori
int **ptrA= (int**)malloc(dim*sizeof(int * ));
int **ptrB= (int**)malloc(dim*sizeof(int * ));
int **ptrC= (int**)malloc(dim*sizeof(int * ));
int * sum =(int *) malloc(sizeof(int));

for(i=0;i<dim;i++){
  ptrA[i]=(int *)malloc(sizeof(int));
  ptrB[i]=(int *)malloc(sizeof(int));
  ptrC[i]=(int *)malloc(sizeof(int));
}


for(i=0;i<dim;i++){
  for(j=0;j<dim;j++){

    ptrA[i][j]=matriceA[i][j];
    ptrB[i][j]=matriceB[i][j];
  }
}

// inizializzazione del mutex
pthread_mutex_init(&mutex,NULL);




// implementazione dei thread
pthread_t singola[dim][dim];
msg *toParent;

//moltiplicazione
for(i=0;i<dim;i++){
  for(j=0;j<dim;j++){
    toParent=(msg *) malloc(sizeof(msg));
    toParent->matA=ptrA;
    toParent->matB=ptrB;
    toParent->matC=ptrC;
    toParent->dimensione=dim;
    toParent->operazione='m';
    toParent->riga=i;
    toParent->colonna=j;

    if(pthread_create(&singola[i][j],NULL,&threadFiglio,(void *)toParent)<0){
      printf("C'è stato un errore nella creazione del thread");
      //esco dal programma perche non riesco a creare le threads.
      exit(0);
    }

  }
}

for(i=0;i<dim;i++){

    toParent=(msg *) malloc(sizeof(msg));
    toParent->matA=ptrA;
    toParent->matB=ptrB;
    toParent->matC=ptrC;
    toParent->dimensione=dim;
    toParent->operazione='s';
    toParent->riga=i;
    toParent->colonna=0;
    toParent->somma=sum;
    if(pthread_create(&singola[i][0],NULL,&threadFiglio,(void *)toParent)<0){
      printf("C'è stato un errore nella creazione del thread");
      //esco dal programma perche non riesco a creare le threads.
      exit(0);
    }


}


for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){
	   pthread_join(singola[i][j],NULL);
	}
}
for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){
	  printf("%i \t",ptrC[i][j] );
    if(j==dim-1){
      printf("\n" );
    }

	}
}

printf("somma vale ==> %i\n",*sum );

for(i=0;i<dim;i++){
  free(ptrA[i]);
  free(ptrB[i]);
  free(ptrC[i]);

}
pthread_mutex_destroy(&mutex);

printf("padre ha finito tutto\n" );


}
