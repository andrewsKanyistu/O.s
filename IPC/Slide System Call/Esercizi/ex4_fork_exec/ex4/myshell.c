/******************************************
MODULO: myshell.c
SCOPO: Mini-shell

La soluzione dell'esercizio e' la stessa vista per l'esercizio 3.
L'unica differenza consiste nel dover distinguere l'esecuzione
di un comando in foreground da quella in background.
Per lanciare un comando in background si inserisce il carattere &
al termine della linea di comando.
Pertanto e' necessario accorgersi della presenza o meno del carattere
& al termine della riga di comando.
Nel caso in cui sia presente il carattere &, la shell non deve attendere
la terminazione del processo figlio creato per eseguire il comando e
deve immediatamente ripresentare il prompt all'utente.
Quando invece & non e' presente, la shell deve attendere tramite la wait
la terminazione del processo figlio prima di accettare nuovi comandi.

******************************************/
