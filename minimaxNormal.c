// Anna Maria Riera Escandell, NIU 1359293. JOC DEL QUATRE EN RATLLA EMPRANT EL MÈTODE DEL MINIMAX
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h> //Per poder usar la funció getche()

#define N 8
#define BUIT ' '
#define ORD 'o' //Ordinador
#define PERS 'x' //Persona
#define PROF 4 //Profunditat de l'arbre, nombre màxim de nivells que aquest pot tenir. Mínim ha de ser 2 (i va de 2 en 2)

#include "minimaxNormal.h"
#include "4ratllaNormal.h"

Node* creaNode(Node *pare, int numFill, int nivell, char jugador){ //Crea un node (valor=0, n_fills, reserva vect fills) des d'un node pare
/*Arguments: qui és el node pare, numFill és sobre quin dels fills que té el pare estic creant el node, nivell del fill que creem i quin jugador és el fill*/
	Node *p = malloc(sizeof(Node));
	p->valor = 0; //Defineix valor com 0
	
	copiaTauler(p-> tauler, pare -> tauler); //Copia el tauler del pare a p
	aplicaTirada(p, numFill, jugador); //Aplica la tirada: el tauler s'actualitza amb la nova "fitxa"
	if (nivell < PROF){ //Si el node a crear NO està a l'últim nivell
		p-> n_fills = numFills(p->tauler); // numFills compta el nombre de fills que pot tenir el node
		if(p->n_fills > 0){
			p-> fills = malloc(p-> n_fills*sizeof(Node*)); //Reservo memòria per al vector de fills: dimensió n_fills, les components són elements *Node
		}
		else{ p-> fills = NULL; } //Si n_fills és 0, el vector de fills és NULL
	}
	else{ p-> n_fills = 0, p-> fills = NULL; } //Si està a l'últim nivell no pot tenir fills
	
	return p;
}
void esborraNode(Node *n){ //Esborra la memòria reservada per a un node (no esborra els fills però sí el vector que els apunta)
	if (n-> n_fills > 0){ //Si n_fills no és 0, vol dir que té vector de fills
		free(n->fills);
		n->fills = NULL;
	}
	free(n);
	n = NULL;
}
void crea1Nivell(Node* pare, int nivell, char jugador){ //Funció que crea un nivell, és a dir, crea tots els fills d'un node. 
/*Arguments: qui és el pare dels fills que volem crear, quin nivell té el pare, jugador*/
	int i;
	for(i=0; i<pare -> n_fills; i++){
		pare -> fills[i] = creaNode(pare, i, nivell+1, jugador);
	}
}
void esborraNivell(Node *pare){ //Esborra tots els fills d'un node (i tot l'arbre de per sota els fills). 
/*Arguments: pare de qui volem borrar-ne els fills*/
	int i, j;
	for(i=0; i<pare->n_fills; i++){ //Si el pare ja està al final de l'arbre no s'entra al bucle i no es fa res
		if(pare->fills[i]->n_fills == 0){ //Si els fills del pare ja són l'últim nivell, els borrem
			esborraNode(pare->fills[i]);	
		}
		else{
			for(j = 0; j<pare->fills[i]->n_fills; j++){
				esborraArbre(pare->fills[i]->fills[j]);
				esborraNode(pare->fills[i]->fills[j]);
			}
			esborraNode(pare->fills[i]);
		}
	}
}
void creaArbre(Node* arrel, int k){ //Funció que a partir d'un node donat crea l'arbre. &&&&& k ha de ser 0
	int i, j;
	
	crea1Nivell(arrel,k, ORD); // Donat el pare (nivell k), crea els nodes fills (tot el nivell k+1)
	for (i=0; i<arrel -> n_fills; i++){
		crea1Nivell(arrel -> fills[i], k+1, PERS); // Per cada fill (nivell k+1), crea els fills dels fills (tot el nivell k+2)
		for (j=0; j<arrel->fills[i]->n_fills; j++){
			if (k+2 == PROF){
				break;
			}
			else if(k+2 < PROF){
				creaArbre(arrel->fills[i]->fills[j], k+2); 
			}
			else{printf("Problema a creaArbre.");}
		}	
	}
}
void esborraArbre(Node* pare){  //Esborra tot l'arbre
	int i;
	for(i=0; i<pare->n_fills; i++){ //Si el pare ja està al final de l'arbre no s'entra al bucle i no es fa res
		if(pare->fills[i]->n_fills == 0){ //Si els fills del pare ja són l'últim nivell, els borrem
			esborraNode(pare->fills[i]);	
		}
		else{
			esborraNivell(pare->fills[i]);
			esborraNode(pare->fills[i]);
		}
	}
}
void recorreArbre(Node *pare, int k){ //Funció que recorre tot l'arbre i l'imprimeix. &&&&& k ha de ser 1
	int i, j;
	
	for(i=0; i<pare->n_fills; i++){ //Anirà fent fins que n_fills = 0, que això vol dir que és l'últim
		printf("Nivell %d (ORD), fill %d, valor = %f.\n", k, i+1, pare->fills[i]->valor);
		for (j=0; j<pare->fills[i]->n_fills; j++){
			printf("   Nivell %d (PERS), fill %d, valor = %f.\n", k+1, j+1, pare->fills[i]->fills[j]->valor);
			recorreArbre(pare->fills[i]->fills[j], k+2);
		}
	}
}
int funcioHeuristica(Node* n, int jugadaPropera){ //Donada una situació del tauler, quant de favorable/desfavorable és
/*Arguments: node i si la jugada és propera al node pare -només es té en compte per al 4 en ratlla- (jugadaPropera = 0, 2, 4, ..., PROF) */
// jugadaPropera = 0 indica que és el pare (hi ha hagut empat), = 2 indica que s'està al nivell 1 o 2, = 4 que s'està al 3 o 4, etc fins PROF
	int puntuacio, i, qratlla, nratlla;
	qratlla = quatreRatlla(n->tauler);
	nratlla = nRatlla(n->tauler);
	for(i=0; i<=PROF; i=i+2){
		if(i==jugadaPropera){ //Amb això: quan menor nivell es tingui (més prop del pare), major la puntuació (més positiva per guanyar, més neg per perdre)
			if(qratlla == 4){ //Si hi ha almenys un 4 en ratlla de PERS
				puntuacio = -1e6 - 100*(PROF-i);
			}
			else if(qratlla == -4){ //Si hi ha almenys un 4 en ratlla de ORD
				puntuacio = 1e6 + 100*(PROF-i);
			}
			else if(nratlla == 5){puntuacio = -5e4;}
			else if(nratlla == 4){puntuacio = -4e3;}
			else if(nratlla == -5){puntuacio = 5e4;}
			else if(nratlla == -4){puntuacio = 4e3;}
			else if(nratlla == 3){puntuacio = -3e2;}
			else if(nratlla == 2){puntuacio = -2e1;}
			else if(nratlla == -3){puntuacio = 3e2 ;}
			else if(nratlla == -2){puntuacio = 2e1 ;}
			else if(nratlla == 1){puntuacio = -1;}
			else if(nratlla == -1){puntuacio = 1;}
			else{puntuacio = 0;}
		}
	}
	return puntuacio;
}

int minim(Node* pare){ //mínim dels valors dels fills d'un node (he d'haver comprovat abans que el node té algun fill, i tots els fills han de tenir valors)
	int min, i;
	
	min = pare->fills[0]->valor;
	for(i=1; i<pare->n_fills; i++){
		if(pare->fills[i]->valor < min){
			min = pare->fills[i]->valor;
		}
		else{
			min = min;
		}
	}
	return min;
}
int maxim(Node* pare){ //màxim dels valors dels fills d'un node (he d'haver comprovat abans que el node té algun fill, i tots els fills han de tenir valors)
	int max, i;
	
	max = pare->fills[0]->valor;
	for(i=1; i<pare->n_fills; i++){
		if(pare->fills[i]->valor > max){
			max = pare->fills[i]->valor;
		}
		else{
			max = max;
		}
	}
	return max;
}
int ompleMinMax(Node* pare, int jugadaPropera){ //Donat el pare, omple els valors de tot l'arbre i retorna pare->valor, jugadaPropera ha de ser 0 &&&&&&&&&&&&&
	int i, j;
	
	if(pare->n_fills == 0){
		pare->valor = funcioHeuristica(pare, jugadaPropera);
	}
	else{ // Notem que el pare no pot tenir 4 en ratlla, pq si això passa s'ha acabat la partida
		for(i=0; i<pare->n_fills; i++){
			if(pare->fills[i]->n_fills == 0){
				pare->fills[i]->valor = funcioHeuristica(pare->fills[i], jugadaPropera+2);
			}
			else{
				if(quatreRatlla(pare->fills[i]->tauler) == -4){ //Si 4 ratlla (ORD) ja no posa valors a l'arbre de baix i valora el node segons el nivell
					pare->fills[i]->valor = funcioHeuristica(pare->fills[i], jugadaPropera+2);
				}
				else{
					for(j=0; j<pare->fills[i]->n_fills; j++){
						if(pare->fills[i]->fills[j]->n_fills == 0){
							pare->fills[i]->fills[j]->valor = funcioHeuristica(pare->fills[i]->fills[j], jugadaPropera+2);
						}
						else{
							if(quatreRatlla(pare->fills[i]->fills[j]->tauler) == 4){ //Si 4 ratlla (PERS) no posa valors a l'arbre de baix i valora segons nivell
								pare->fills[i]->fills[j]->valor = funcioHeuristica(pare->fills[i]->fills[j], jugadaPropera+2);
							}
							else{
								pare->fills[i]->fills[j]->valor = ompleMinMax(pare->fills[i]->fills[j], jugadaPropera+2);
							}
						}
					}  
					pare->fills[i]->valor = minim(pare->fills[i]); //Mínim dels valors dels fills de fills[i]
				}
			}
		}
		pare->valor = maxim(pare); //Màxim dels valors dels fills de pare
	}
	return pare->valor;
}
int miniMax(Node* pare){ //Donat el pare, torna la columna on millor tirar segons el mètode del minimax
	int max, i, colMax, fillTirada, j; //fillTirada és el nombre fill que té el màxim valor on es tirarà, colMax la columna a qui correspon
	int fillsMax[N]; //vector on estaran tots els fills amb els màxims valors (n'hi haurà un o més)
	creaArbre(pare, 0); //Creo l'arbre sencer
	max = ompleMinMax(pare, 0); // Omplo els "valor", que és l'única variable que falta omplir i determino el valor màxim
	//recorreArbre(pare, 1);
	//printf("\nMaxim = %d", max);
	for(j=0; j<N; j++){ // Inicialitzo tot el vector fillsMax
		fillsMax[j] = -1; //Inicialitzo així per si faig un for per als fillsMax, així quan arribi a -1 vol dir que no n'hi ha més
	}
	j=0;
	for(i=0; i<pare->n_fills; i++){ //Trobem els fills que tenen el valor màxim (totes les jugades són igual de bones)
		if(pare->fills[i]->valor == max){
			fillsMax[j] = i;
			j = j+1;
		}
	}
	fillTirada = fillsMax[0]; //Tira a la primera de les opcions amb la màxima puntuació
	colMax = determinaColumna(pare, fillTirada); //Determino la columna a què correspon el fill on es dóna el màxim
	esborraArbre(pare); //Esborro l'arbre sencer
	return colMax;
}