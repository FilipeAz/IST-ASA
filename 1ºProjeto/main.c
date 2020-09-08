/* ---------------------------------------------------- Projeto de ASA --------------------------------------------------------------
 
 Filipe Azevedo - nº 82468
 Martim Zanatti - nº 82517 

-----------------------------------------------------------------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* -------------------------------------------------------  Vertices --------------------------------------------------------------*/

/* Estrutura */

typedef struct vertices{
    
    int id;
    int visited;
    int profundidade;
	int low;
	int nrFilhos;
    int fundamental;
	int parentId;

}Vertice;

/* ------------------- */

/* Construtor */ 

Vertice criaVertice(int id){
	
	Vertice v;
	v.id = id;
	v.visited = 0;
	v.profundidade = 0;
	v.low = 0;
	v.nrFilhos = 0;
	v.fundamental = 0; 
	v.parentId = 0;
	
	return v;
}

/* ------------------- */

/* Getters/Setters */

int getID(Vertice v){
	
	return v.id;
}

int getVisited(Vertice v){
	
	return v.visited;
}

int getProfundidade(Vertice v){
	
	return v.profundidade;
}

int getLow(Vertice v){
	
	return v.low;
}

int getNrFilhos(Vertice v){
	
	return v.nrFilhos;
}

int getFundamental(Vertice v){
	
	return v.fundamental;
}

int getParentID(Vertice v){
	
	return v.parentId;
}

void setVisited(Vertice *v, int i){
	
	v->visited = i;
}

void setProfundidade(Vertice *v, int i){
	
	v->profundidade = i;
}

void setLow(Vertice *v, int i){
	
	v->low = i;
}

void setParentID(Vertice *v, int id){
	
	v->parentId = id;
}

void setnrFilhos(Vertice *v){
	
	v->nrFilhos++;
}

void resetNrFilhos(Vertice *v){
	
	v->nrFilhos = 0;
}

void setFundamental(Vertice *v, int i){
	
	v->fundamental = i;
}

/* ------------------- */ 

/* --------------------------------------------------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------ Lista -------------------------------------------------------------*/

/* Estrutura */

typedef struct LISTAnode* linkLISTA;

struct LISTAnode {
    
    Vertice vertice;
    linkLISTA next;
};

/* ------------------- */ 

/* Construtores */ 

linkLISTA geraLista(int dim){
	
	int i;
    linkLISTA lista = malloc(dim*sizeof(struct LISTAnode)); 
	for(i = 0;i < dim;i++){
		lista[i].next = NULL;
	}
	
	return lista;
}

linkLISTA NEW(Vertice v, linkLISTA next) {
	
	linkLISTA x = (linkLISTA) malloc(sizeof(struct LISTAnode));
	x->vertice = v;
	x->next = next;
	
	return x;
} 

/* ------------------- */ 

/* Métodos */

linkLISTA addVertice(linkLISTA lista, Vertice v){
	
	lista[getID(v) - 1].vertice = v;
	return lista;
}

linkLISTA criaLigacao(linkLISTA lista, Vertice v, Vertice w){
	
	linkLISTA wNode = NEW(w,lista[getID(v) - 1].next);
	linkLISTA vNode = NEW(v,lista[getID(w) - 1].next);
	lista[getID(v) - 1].next = wNode;
	lista[getID(w)- 1].next = vNode;
	
	return lista;
}

linkLISTA verticesFundamentais(linkLISTA lista, Vertice v, int depth){
	
	int vID = getID(v);
	linkLISTA l=lista[vID -1].next;
	setVisited(&lista[vID-1].vertice,1);
	setProfundidade(&lista[vID-1].vertice,depth);
	setLow(&lista[vID-1].vertice,depth);
	resetNrFilhos(&lista[vID-1].vertice);
	setFundamental(&lista[vID-1].vertice,0);
	while(l != NULL){
		Vertice w = l->vertice;
		int wID = getID(w); 
		if(!getVisited(lista[wID-1].vertice)){
			setParentID(&lista[wID-1].vertice,getID(v));
			lista = verticesFundamentais(lista, lista[wID-1].vertice, depth + 1);
			setnrFilhos(&lista[vID-1].vertice);
			if(getLow(lista[wID-1].vertice) >= getProfundidade(lista[vID-1].vertice))
				setFundamental(&lista[vID-1].vertice,1);
			(getLow(lista[vID-1].vertice) <= getLow(lista[wID-1].vertice)) ? setLow(&lista[vID-1].vertice,getLow(lista[vID-1].vertice)) : setLow(&lista[vID-1].vertice,getLow(lista[wID-1].vertice));
		}
		else if(getID(w) != getParentID(v))
			(getLow(lista[vID-1].vertice) <= getProfundidade(lista[wID-1].vertice)) ? setLow(&lista[vID-1].vertice,getLow(lista[vID-1].vertice)) : setLow(&lista[vID-1].vertice,getProfundidade(lista[wID-1].vertice));
		l = l->next;
	}
	if((getParentID(lista[vID-1].vertice) != 0 && getFundamental(lista[vID-1].vertice) == 1) || (getParentID(lista[vID-1].vertice) == 0 && getNrFilhos(lista[vID-1].vertice) > 1));
	else{
		setFundamental(&lista[vID-1].vertice,0);
	}
	return lista;
}


void outPut(linkLISTA lista,int dim){
	
	int m = -1, M = -1, numVF = 0, i;
	for(i = 0; i < dim; i++){
		if(getFundamental(lista[i].vertice) == 1){
			if(numVF == 0)
				m = i+1;
			M = i+1;
			numVF++;
		} 
	}
	printf("%d\n%d %d\n", numVF, m, M);
}

/* ------------------- */ 

/* Libertação de Memoria */

void rmv(linkLISTA lista, int dim){
	
	int i;
	for(i=0;i<dim;i++){
		linkLISTA l=lista[i].next,aux;
		while(l!=NULL){
			aux=l->next;
			free(l);
			l=aux;
		}
	}

	free(lista);
}

/* ------------------- */ 

/* --------------------------------------------------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------ Main --------------------------------------------------------------*/

int main(){
	
	clock_t t = clock();
	int vertices, ligacoes, i, idV, idW;
	Vertice v, w;
	linkLISTA lista;
	scanf("%d %d",&vertices,&ligacoes);
	lista = geraLista(vertices);
	for(i=0;i<ligacoes;i++){
		scanf("%d %d", &idV, &idW);
		v = criaVertice(idV);
		w = criaVertice(idW);
		lista = addVertice(lista,v);
		lista = addVertice(lista,w);
		lista = criaLigacao(lista, v, w);
	}
	
	lista = verticesFundamentais(lista,lista[0].vertice,1);
	
	outPut(lista,vertices);

	rmv(lista,vertices);
	
	t = clock() - t;
	
	printf("%f\n", ((double)t)/CLOCKS_PER_SEC);
	
	return 0;
}

/* --------------------------------------------------------------------------------------------------------------------------------*/
