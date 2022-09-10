#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT_MAX 4294967295


typedef struct {
	int index;
	unsigned int lunghezzaMST;
}statsGrafo;


void relax_node(unsigned int dist[], int inMST[], int dim, unsigned int Grafo[dim][dim], int pred[], int cont) {	
	//guardiamo i nodi adiacenti al nodo corrente e prendiamo quello con distanza minore, aggiungendolo al MST
	unsigned int u;
	int min_index = 0;
	unsigned int node_dis = INT_MAX;
	
	for (int v = 0; v < dim; v++) {
		if (inMST[v] == 0 && dist[v] <= node_dis) {
			min_index = v;
			node_dis = dist[v];
		}
	}
	
	u = min_index;
	
	inMST[u] = 1;	//aggiunto al MST
	
	//aggiorna l'array delle distanze minime
	for(int v = 0; v < dim; v++) {
		if(inMST[v] == 0 && Grafo[u][v] && dist[u] != INT_MAX && dist[u] + Grafo[u][v] < dist[v]) {
			dist[v] = dist[u] + Grafo[u][v];
			pred[cont] = u;
		}
	}
}


int dijkstra (int sorgente, int dim) {
	unsigned int dist[dim];	//array contenente le distanze di ogni nodo dal nodo sorgente
	
	unsigned int MST;	//valore totale dei cammini minimi
	
	int inMST[dim];	//vale 1 se il nodo è già stato aggiunto all'albero dei cammini minimi, 0 altrimenti
	
	int pred[dim];		//vettore dei nodi precedenti
	
	//leggiamo il grafo
	unsigned int Grafo[dim][dim];
	
	int i = 0;
	char charLetto;
	int chartoInt;
	unsigned int daInserire;
	
	while (i < dim) {
		for(int j = 0; j < dim; j++) {
			daInserire = 0;
			charLetto = getchar_unlocked();
			
			while(charLetto != ',' && charLetto != '\n') {	
				chartoInt = 0;
				chartoInt = charLetto - '0';
				daInserire = daInserire * 10 + chartoInt;
				charLetto = getchar_unlocked();
			
		      	}
		      	Grafo[i][j] = daInserire;
 		}
 		i++;
 	}
 	
	//controlliamo il caso base: nessun nodo è raggiungibile da 0
	int baseCase = 0;
	bool isbaseCase = false;
	
	for(int i = 1; i < dim && baseCase == 0; i++)
		baseCase = baseCase + Grafo[0][i];
		
	if(baseCase == 0)
		isbaseCase = true;
	
	if(isbaseCase == false) {
		//inizializzo tutte le distanze dal nodo sorgente a infinito e tutti i nodi non appartengono al MST
		for(int i = 0; i < dim; i++) {
			dist[i] = INT_MAX;
			inMST[i] = 0;
			pred[i] = 0;
		}
		
		//il nodo sorgente ha sempre distanza da se stesso pari a 0
		dist[sorgente] = 0;
		
		//cerco il percorso più breve verso ogni nodo
		for (int cont = 0; cont < dim - 1; cont++)
			relax_node(dist, inMST, dim, Grafo, pred, cont);
		
		MST = 0;
		
		for(int i = 0; i < dim; i++)
			MST = MST + dist[i];
	}
	else
		MST = 0;
	
	return(MST);
}


int Parent(int i) {
	return((i - 1) / 2);
}


void swap(statsGrafo* x, statsGrafo* y) {
	statsGrafo temp;
	temp = *y;
	*y = *x;
	*x = temp;
}


void maxHeapify(statsGrafo classifica[], int dim, int n) {
	int posmax = n;
	int left = 2*n + 1;
	int right = 2*n + 2;
	
	if(left < dim && classifica[left].lunghezzaMST > classifica[n].lunghezzaMST)
		posmax = left;
	else
		posmax = n;
	if(right < dim && classifica[right].lunghezzaMST > classifica[posmax].lunghezzaMST)
		posmax = right;
		
	if(posmax != n) {
		swap(&classifica[n], &classifica[posmax]);
		maxHeapify(classifica, dim, posmax);
	}
}


void stampaClassifica(statsGrafo classifica[], int dim) {
	int i = 0;
	
	printf("%d", classifica[i].index);
	i++;
	while(i < dim) {
		printf(" %d", classifica[i].index);
		i++;
	}
	printf("\n");
}


void inserisci(statsGrafo classifica[], int dim, int mstLetto, int indice) {
	int i;
	classifica[dim].lunghezzaMST = mstLetto;
	classifica[dim].index = indice;
	i = dim;
	
	while(i != 0 && classifica[Parent(i)].lunghezzaMST < classifica[i].lunghezzaMST) {
		swap(&classifica[Parent(i)], &classifica[i]);
		i = Parent(i);
	}
}
	
	
void eliminaMax (statsGrafo classifica[], int dim) {
	classifica[0] = classifica[dim - 1];
	dim = dim - 1;
	maxHeapify(classifica, dim, 0);
}


int main() {
	int numNodi;	//numero di nodi per ogni grafo
	int topK;	//numero di grafi da mettere in classifica
	int i;	//indice che aumenta a ogni grafo aggiunto
	char c;	//lettura primo carattere di ogni nuova istruzione
	unsigned int mstLetto;	//variabile che a goni lettura mantiene il valore MST calcolato da Dijkstra
	bool isK0 = false;
	bool topkNotcalled = true;
	
	//leggo i due valori iniziali, dati un'unica volta
	if(scanf("%d %d\n", &numNodi, &topK)){};
	
	statsGrafo classifica[topK];
	
	if(topK == 0)
		isK0 = true;
	
	i = 0;
	
	c = getchar_unlocked();
	
	while (c != EOF)	{	//da qui iniziano le varie operazioni
		if (c == 'A') {		//leggiamo "AggiungiGrafo"
			while(c != '\n')
				c = getchar_unlocked();	//elimino tutta la stringa letta dopo la 'A' di AggiungiGrafo e vado alla riga successiva
			
			mstLetto = dijkstra(0, numNodi);
			
			if(i < topK)
				inserisci(classifica, i, mstLetto, i);
			else {
				int mstMax = classifica[0].lunghezzaMST;
				if(mstMax > mstLetto) {
					eliminaMax(classifica, topK);
					inserisci(classifica, topK-1, mstLetto, i);
				}
			}	
			i++;
		}
		
		if (c == 'T') {		//leggiamo "TopK"
			while(c != '\n')
				c = getchar_unlocked();
			
			topkNotcalled = false;
			
			if(isK0 == true)
				printf("\n");
				
			
			if(i == 0)
				printf("\n");
			else if(i < topK)
				stampaClassifica(classifica, i);
			else
				stampaClassifica(classifica, topK);
		}
				
		c = getchar_unlocked();
	}
	
	if(topkNotcalled == true)
				printf("\n");
				
	return(0);
}