#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <ftw.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <wait.h>
#include <pthread.h>

char memory[100];

#define DIM_MATRICE atoi(argv[4])
#define NUM_PROCESSI atoi(argv[5])

pthread_mutex_t lock;





char c;	                        //buffer per leggere una lettera alla volta
int i=0, j=0, n=0;	            //indice per il vettore dei caratteri user
int fd;                         //file descriptor
int nread;                      //numero caratteri letti
int count=0;                    //contatore per verificare la grandezza della matrice
long where;                     //posizione all'interno del file
int matrice[10][10];            //matrice per la gestione della memoria condivisa (a matrice)
int contatore=0;
int totale=0;
int h=0;

/* generazione chiavi per la memoria condivisa */
key_t chiaveA;
key_t chiaveB;
key_t chiaveC;
key_t chiaveD;
key_t chiaveMemoria;

/* generazione identificatori canali per la memoria condivisa */
int memA;
int memB;
int memC;
int memD;
int memCondivisa;

//----------------------------------------------------------------------------------- INIZIO MAIN
int main (int argc, char *argv[]){

  typedef struct {
    int       id;       /* id della thread */
    int       size; /* dimensione della matrice */
    int       riga; /* indice della riga */
    int       colonna; /* indice della colonna */
    int       (*A)[atoi(argv[4])]; /*indice della riga */
    int       (*B)[atoi(argv[4])]; /* indice della colonna */
    int       (*C)[atoi(argv[4])]; /* indice della colonna */
    int       *D; /* indice della colonna */
  } package_t;




  void deallocazione();
  void deallocazioneA();
  void deallocazioneB();
  void deallocazioneC();
  void *mult_worker();
  void *somma();
  pthread_t threads[NUM_PROCESSI];       /* per tenere traccia degli identificatori delle thread */
  /*viene creata una thread per ogni riga*/

  package_t *valore_thread;             /* argomento per le thread */


  //matrici per la generazione della memoria condivisa a forma matriciale
  int matriceA[DIM_MATRICE][DIM_MATRICE];
  int matriceB[DIM_MATRICE][DIM_MATRICE];
  int matriceC[DIM_MATRICE][DIM_MATRICE];

  /*controllo degli argomenti*/
  if(argc != 6)){
    write(1, "Inserire un numero corretto di valori\n", sizeof("Inserire un numero corretto di valori\n"));
    write(1, "eseguibile  -  matriceA  -  matriceB  -  matriceC  -  dimensione  -  numero di processi\n", sizeof("eseguibile  -  matriceA  -  matriceB  -  matriceC  -  dimensione  -  numero di processi\n"));
    return -1;
  }

  //creazione delle chiavi
  chiaveA=ftok(argv[1], 'f');
  chiaveB=ftok(argv[2], 'e');
  chiaveC=ftok(argv[3], 'g');
  chiaveD=ftok("finale.c", 'h');
  chiaveMemoria=IPC_PRIVATE;

  //--------------------------------------------------------------------------------------- MEMORIA CONDIVISA

  // CREAZIONE MEMORIA CONDIVISA PER LA PRIMA MATRICE
  memA = shmget(chiaveA, sizeof(matriceA), 0644|IPC_CREAT)<0){
    write(1, "fallita la creazione della memoria condivisa associata alla prima matrice" , sizeof("fallita la creazione della memoria condivisa associata alla prima matrice"));
    deallocazione_semaforo();
    return 1;
  }
  int (*p_A)[DIM_MATRICE];
  p_A=shmat(memA, NULL, 0);


  // CREAZIONE MEMORIA CONDIVISA PER LA SECONDA MATRICE
  memB = shmget(chiaveB, sizeof(matriceB), 0644|IPC_CREAT)<0){
    write(1, "fallita la creazione della memoria condivisa associata alla seconda matrice" , sizeof("fallita la creazione della memoria condivisa associata alla seconda matrice"));
    deallocazioneA();
    return 1;
  }
  int (*p_B)[DIM_MATRICE];
  p_B=shmat(memB, NULL, 0);


  //CREAZIONE MEMORIA CONDIVISA PER LA TERZA MATRICE (MATRICE RISULTANTE)
  memC = shmget(chiaveC, sizeof(matriceC), 0644|IPC_CREAT)<0){
    write(1, "fallita la creazione della memoria condivisa associata alla terza matrice" , sizeof("fallita la creazione della memoria condivisa associata alla terza matrice"));
    deallocazioneB();
    return -1;
  }
  int (*p_C)[DIM_MATRICE];
  p_C=shmat(memC, NULL, 0);


  //CREAZIONE MEMORIA CONDIVISA PER LA SOMMA
  if(memD = shmget(chiaveD, sizeof(int), 0666|IPC_CREAT)<0){
    write(1, "fallita la creazione della memoria condivisa associata alla somma" , sizeof("fallita la creazione della memoria condivisa associata alla somma"));
    deallocazioneC();
    return -1;
  }
  int *p_D;
  p_D=(int *)shmat(memD, NULL, 0666);
  *p_D=0;
  //--------------------------------------------------------------------------------------- MEMORIA CONDIVISA


  //--------------------------------------------------------------------------------------- RIEMPIMENTO MEMORIA CONDIVISA A

  //stampa prima matrice a video e la salvo nella prima zona di memoria condivisa
  write(1, "\n \n  MATRICE A \n \n",sizeof("\n \n  MATRICE A \n \n"));

  if(fd=open(argv[1],O_RDONLY)<0){
    write(1, "ERRORE nell'apertura del file 1", sizeof("ERRORE nell'apertura del file 1"));
    deallocazione();
    return -1;
  }

  while((nread=read(fd, &c, 1))==1){

    if( c!= ' ' &&  c!= '\n'){
      n=n+atoi(&c);
      matriceA[j][i]=n;
      n=n*10;
    }
    if(c== ' '){
      i++;
      n=0;
    }

    if(c== '\n'){
      i=0;
      j++;
      n=0;
    }

    if(i==(DIM_MATRICE+1) || j==(DIM_MATRICE+1)){
      write(1, "ERRORE - La dimensione della matrice A non corrisponde", sizeof("ERRORE - La dimensione della matrice A non corrisponde") );
      deallocazione();
      return -1;
    }
    write(1, &c, 1);
  }
  //--------------------------------------------------------------------------------------- RIEMPIMENTO MEMORIA CONDIVISA A
  i=0, n=0,j=0; //re-inizializzo i contatori
  //--------------------------------------------------------------------------------------- RIEMPIMENTO MEMORIA CONDIVISA B

  //stampa seconda matrice a video e la salvo nella seconda zona di memoria condivisa
  write(1, "\n \n  MATRICE B \n \n",sizeof("\n \n  MATRICE B \n \n"));

  if(fd=open(argv[2],O_RDONLY)<0){
    write(1, "ERRORE nell'apertura del file 2", sizeof("ERRORE nell'apertura del file 2"));
    deallocazione();
    return -1;
  }

  while((nread=read(fd, &c, 1))==1){

    if( c!= ' ' &&  c!= '\n'){
      n=n+atoi(&c);
      matriceB[j][i]=n;
      n=n*10;
    }
    if(c== ' '){
      i++;
      n=0;
    }

    if(c== '\n'){
      i=0;
      j++;
      n=0;
    }

    if(i==(DIM_MATRICE+1) || j==(DIM_MATRICE+1)){
      write(1, "ERRORE - La dimensione della matrice B non corrisponde", sizeof("ERRORE - La dimensione della matrice B non corrisponde") );
      deallocazione();
      return -1;
    }
    write(1, &c, 1);
  }

  /*associo le matrici alle zone della memoria condivisa*/
  for(i=0; i<DIM_MATRICE; i++){
    for(j=0; j<DIM_MATRICE; j++){
      p_A[i][j]=matriceA[i][j];
      p_B[i][j]=matriceB[i][j];
    }
  }

  //--------------------------------------------------------------------------------------- RIEMPIMENTO MEMORIA CONDIVISA B

  //--------------------------------------------------------------------------------------------------    CREAZIONE THREAD
  int NUM_PROCESSI_MODIFICATI=0;

  if(NUM_PROCESSI>DIM_MATRICE){
    NUM_PROCESSI_MODIFICATI=DIM_MATRICE;
  }else{
    NUM_PROCESSI_MODIFICATI=NUM_PROCESSI;
  }


  int sum=0, counter=0, RIGA=0, COLONNA=0, variabile=0;

  //-------------------------------------------------------------------    MOLTIPLICAZIONE

  pthread_mutex_init(&lock, NULL);
  write(1,"\n\n\nMOLTIPLICAZIONE\n", sizeof("\n\n\nMOLTIPLICAZIONE\n"));
  for(RIGA=0; RIGA<DIM_MATRICE; RIGA++){
    for(COLONNA=0; COLONNA<DIM_MATRICE; COLONNA++){

      valore_thread = (package_t *)malloc(sizeof(package_t));
      valore_thread->id = variabile;
      valore_thread->size = DIM_MATRICE;
      valore_thread->A=p_A;
      valore_thread->B=p_B;
      valore_thread->C=p_C;
      valore_thread->D=p_D;
      valore_thread->riga = RIGA;
      valore_thread->colonna = COLONNA;
      pthread_create(&threads[variabile], NULL, mult_worker, valore_thread);

      if (variabile==(NUM_PROCESSI_MODIFICATI-1)){
        variabile=0;
      }else{
        variabile++;
      }
    }
  }

  for (h = 0; h < NUM_PROCESSI_MODIFICATI; h++) {
    pthread_join(threads[h], NULL);
    write(1, "THREAD: il figlio è nel join\n", sizeof("THREAD: il figlio è nel join\n"));
  }

  pthread_mutex_destroy(&lock);


//stampo a video la matrice C
  write(1,"\n\n MATRICE C \n\n",sizeof("\n\n MATRICE C \n\n"));

  for(RIGA=0; RIGA<DIM_MATRICE; RIGA++){
    for(COLONNA=0; COLONNA<DIM_MATRICE; COLONNA++){
      sprintf(memory, "%d ", p_C[RIGA][COLONNA]);
      write(1, memory, sizeof(memory));
    }
    write(1, "\n", sizeof("\n"));
  }


//stampo a video la somma
  char valore_somma[10];
  sprintf(valore_somma,"somma %d\n", *p_D);
  write(1,valore_somma, sizeof(valore_somma));



  //chiusura memoria condivisa
  deallocazione();

}

void somma(int C, int  *D, int riga) {
  *D+=C;
  printf("SOMMA THREAD : processing A riga %d, B colonna %d, numero %d,,,, %d\n",riga, i,C, *D);
}

void *mult_worker( void *arg) {

  package_t *p = (package_t *)arg;
  int y=0, MOLTIPLICAZIONE=0;
  for(y=0; y < p->size ;y++){
    MOLTIPLICAZIONE=MOLTIPLICAZIONE+(p->A[p->riga][y] * p->B[y][p->colonna]);
  }

  p->C[p->riga][p->colonna]=MOLTIPLICAZIONE;
  printf("\nMOLTIPLICAZIONE %d\n", MOLTIPLICAZIONE);
  pthread_mutex_lock(&lock);
  somma(MOLTIPLICAZIONE, p->D, p->riga );
  printf("MATRIX THREAD %d: processing A riga %d, B colonna %d, numero %d\n", p->id, p->riga, p->colonna,p->A[p->riga][p->colonna] );
  pthread_mutex_unlock(&lock);
  free(p);
  pthread_exit(NULL);


}





void deallocazioneA(){
  shmdt(NULL);

  if(shmctl(memA, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla prima matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla prima matrice"));
  }
}


void deallocazioneB(){
  shmdt(NULL);

  if(shmctl(memA, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla prima matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla prima matrice"));
  }

  if(shmctl(memB, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla seconda matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla seconda matrice"));
  }

}


void deallocazioneC(){
  shmdt(NULL);

  if(shmctl(memA, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla prima matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla prima matrice"));
  }

  if(shmctl(memB, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla seconda matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla seconda matrice"));
  }

  if(shmctl(memC, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'elieminazione della memoria associata alla terza matrice (moltiplicazione)", sizeof("ERRORE nell'eliminazione della memoria associata alla terza matrice (moltiplicazione)"));
  }

}

void deallocazione(){
  shmdt(NULL);

  if(shmctl(memA, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla prima matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla prima matrice"));
  }

  if(shmctl(memB, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'eliminazione della memoria associata alla seconda matrice", sizeof("ERRORE nell'eliminazione della memoria associata alla seconda matrice"));
  }

  if(shmctl(memC, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'elieminazione della memoria associata alla terza matrice (moltiplicazione)", sizeof("ERRORE nell'eliminazione della memoria associata alla terza matrice (moltiplicazione)"));
  }

  if(shmctl(memD, IPC_RMID, NULL)==-1){
    write(1, "ERRORE nell'elieminazione della memoria associata alla somma", sizeof("ERRORE nell'eliminazione della memoria associata alla somma"));
  }
}
