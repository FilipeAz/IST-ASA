#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


/* -------------------------------------------------------  Vertices --------------------------------------------------------------*/

/* Estrutura */

typedef struct vertices{
    
    int id;
    int distancia;
	int distanciaTOT;
	int parentId;
	int peso;
	int filial;

}Vertice;

/* ------------------- */

/* Construtor */ 

Vertice criaVertice(int id){
	
	Vertice v;
	v.id = id;
	v.distancia = INT_MAX;
	v.distanciaTOT = 0;
	v.parentId = 0;
	v.peso = 0;
	v.filial = 0;
	
	return v;
}

/* ------------------- */

/* Getters/Setters */

int getID(Vertice v){
	
	return v.id;
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



linkLISTA NEW(Vertice v, linkLISTA next){
	
	linkLISTA x = (linkLISTA) malloc(sizeof(struct LISTAnode));
	x->vertice = v;
	x->next = next;
	
	return x;
} 

linkLISTA criaLigacao(linkLISTA lista, int idV, int idW, int p){
	Vertice w;
	w = criaVertice(idW);
	linkLISTA wNode = NEW(w,lista[idV - 1].next);
	wNode->vertice.peso = p;
	lista[idV - 1].next = wNode;
	
	return lista;
}

linkLISTA geraLista(int dim){
	
	int i;
	Vertice v;
    linkLISTA lista = malloc(dim*sizeof(struct LISTAnode)); 
	for(i = 0;i < dim;i++){
		v = criaVertice(i+1);
		lista[i].vertice = v;
		lista[i].next = NULL;
	}
	return lista;
}

/* ------------------- */ 

/* Métodos */

linkLISTA setFilial(linkLISTA lista, int idFilial){
	
	lista[idFilial - 1].vertice.filial = 1;
	return lista;
}


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

linkLISTA bellmanFord(linkLISTA lista,int idS,int localidades){
	int v, w, i, verifica = 0;
   	linkLISTA a;	
   	for (w = 0; w < localidades; w++){
      	lista[w].vertice.parentId = 0;
      	lista[w].vertice.distancia = INT_MAX;
	}
   	lista[idS - 1].vertice.parentId = idS; 
   	lista[idS - 1].vertice.distancia = 0;

	for(i = 0;i<localidades-1;i++){
		for (v = 0; v < localidades; v++){
			for (a = lista[v].next; a != NULL; a = a->next){
				if(lista[v].vertice.distancia != INT_MAX){
					if (lista[v].vertice.distancia + a->vertice.peso < lista[a->vertice.id - 1].vertice.distancia) {
						lista[a->vertice.id - 1].vertice.distancia = lista[v].vertice.distancia + a->vertice.peso;
						lista[a->vertice.id - 1].vertice.parentId = v + 1;
						verifica = 1;
					}
				}
			}   
		}
		if(verifica == 0){
			break;
		}
		verifica = 0;
	}		
      
return lista;

}

linkLISTA antibellmanFord(linkLISTA lista,int idS,int localidades){
	int v, w, i, verifica = 0;
   	linkLISTA a;	
   	for (w = 0; w < localidades; w++){
      	lista[w].vertice.parentId = 0;
      	lista[w].vertice.distancia = INT_MAX;
	}
   	lista[idS - 1].vertice.parentId = idS; 
   	lista[idS - 1].vertice.distancia = 0;

	for(i = 0;i<localidades-1;i++){
		for (v = 0; v < localidades; v++){
			for (a = lista[v].next; a != NULL; a = a->next){
				if(lista[a->vertice.id - 1].vertice.distancia != INT_MAX){
					if (lista[a->vertice.id - 1].vertice.distancia + a->vertice.peso < lista[v].vertice.distancia) {
						lista[v].vertice.distancia = lista[a->vertice.id - 1].vertice.distancia + a->vertice.peso;
						lista[v].vertice.parentId = a->vertice.id;
						verifica = 1;	
					}
				}
			}   
		}
		if(verifica == 0){
			break;
		}
		verifica = 0;
	}		
      
return lista;

}



/* ------------------- */ 

/* --------------------------------------------------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------ Main --------------------------------------------------------------*/

int main(){
	
	clock_t t = clock();
	int localidades, filiais, idFilial, nArcos,i = 0, idV, idW, peso, j, min = INT_MAX,id = 0/*, aux = 0*/;
	linkLISTA lista;
	scanf("%d %d %d",&localidades,&filiais,&nArcos);
	lista = geraLista(localidades);
	int prioridade[filiais];
	while(i < filiais && scanf("%d ", &idFilial) == 1){
		prioridade[i] = idFilial;
		lista = setFilial(lista,idFilial);
		i++;
	}

	for (i = 0; i < nArcos; i++){
		scanf("%d %d %d", &idV, &idW, &peso);
	
		lista = criaLigacao(lista, idV,idW,peso);
	}

	

	for(i = 0;i < filiais;i++){
		lista = bellmanFord(lista,prioridade[i],localidades);
		for (j = 0; j < localidades; j++){
			if(lista[j].vertice.distancia == INT_MAX){
				lista[j].vertice.distanciaTOT = INT_MAX;
			}
			if(lista[j].vertice.distanciaTOT != INT_MAX){
				lista[j].vertice.distanciaTOT += lista[j].vertice.distancia;
			}
		}	
	}	

	


	for(i = 0;i<localidades;i++){
		if(lista[i].vertice.distanciaTOT<min){
			min = lista[i].vertice.distanciaTOT;
			id = i+1;
		}
	}
	if(min==INT_MAX){
		printf("N\n");
		rmv(lista,localidades);
		t = clock() - t;
	
		printf("%f\n", ((double)t)/CLOCKS_PER_SEC);
		return 0;
	}
	printf("%d %d\n",id,min);
	lista = antibellmanFord(lista,id,localidades);
	for(i = 0;i<filiais;i++){
		printf("%d ", lista[prioridade[i] - 1].vertice.distancia);
		}
	
	printf("\n");
	rmv(lista,localidades);
	
	t = clock() - t;
	
	printf("%f\n", ((double)t)/CLOCKS_PER_SEC);
	
	return 0;
}

/* --------------------------------------------------------------------------------------------------------------------------------*/

