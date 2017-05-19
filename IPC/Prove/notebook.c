	int fd_out; 		// file desc
	char temp;
	int dim=3;				// dimensione matrice
	char caratteri[100];
	char tempNum[10];
	int index=0;
	
	int i=0,j=0;			//indici per la matrice
	int matriceA [dim][dim];
	int quantiTab=0;
	char riga[100];
	key_t chiave = ftok("es2.c",getpid());
	int shmid;
	if((shmid=shmget(chiave,1000,IPC_CREAT|IPC_EXCL|0666))< 0){
		printf("errore shmget\n");
	}
	printf("sono nel padre\n");
	
	printf("sono il padre il mio pid %d\n",getpid());
	wait();
	int figlio=fork();
	int qualsiasivariabile=0;
	
	if(figlio==0){
		printf("sono nel figlio il mio padre è %d\n",getppid());
		sleep(5);
		printf("sono sveglio\n");
		qualsiasivariabile=getpid();
		
	}
	sleep(10);
	kill(qualsiasivariabile,SIGKILL);
	

	if (argc <0)
	{
		printf("numero di argomenti non sufficienti\n");
	}
	if ((fd_out=open(argv[1],O_RDONLY))==-1)
	{
		printf("file nn letto\n");
	}
	
	//char tempChars[10]="";
	int pos=0, numeroTemp=0;
	//int readChars=0;		//caratteri letti dai file
	int curNum=0,readChars=0,count=0;


	while ((read(fd_out,&temp,1))==1){	
		if(temp != ' ' && temp != '\n'){
			curNum=curNum+atoi(&temp);
			matriceA[i][j]=curNum;
			curNum=curNum*10;
		}
		if(temp==' '){
			i++;
			curNum=0;
		}
		if (temp == '\n')
		{
			j++;
			curNum=0;
			i=0;
		}
		if (i >= dim+1 || j>= dim+1){
			return -1;
		}
		count++;
		caratteri[count]=temp;
	}
	
	
	
	
	
	write(1,caratteri,count+1);