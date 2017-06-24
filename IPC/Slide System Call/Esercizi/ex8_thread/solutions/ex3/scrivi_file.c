#include <stdio.h>
#include <time.h>


int main(int arg, char *args[]) {

	FILE *f;
	char *filename;
	short i, n;
	int tmp;
	double num;
	
	if(arg != 2)
		printf("\nUsage: scrivi_file.x <filename>\n\n");
	else {
		srand(time(NULL));
		printf("Digitare il numero di elementi da generare: ");
	
		scanf("%hd", &n);
	
		filename = args[1];
		f = fopen(filename, "w");
	
		for(i=0; i<n; i++) {
				num = rand() % n; /* genera un numero casuale compreso tra 0 e n*/
			fprintf(f, "%d\n", (int)num);
		}	

		fclose(f);
	
		printf("Generazione effettuata. I valori sono stati salvati sul file %s\n\n",filename);
	}		
}
