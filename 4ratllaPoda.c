// Anna Maria Riera Escandell, NIU 1359293. JOC DEL QUATRE EN RATLLA EMPRANT LA PODA ALFA-BETA DEL MÈTODE DEL MINIMAX
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

#include "minimaxPoda.h"
#include "4ratllaPoda.h"

void inicialitzaTauler(Node *n){ //Inicialitza el tauler omplint-lo tot de "BUIT"
	int i, j;
	for (i=0 ; i<N ; i++){ 
		for (j=0 ; j<N ; j++){
			n->tauler[i][j] = BUIT;
		}
	}
}
void inicialitzaNode(Node *nodeInicial){  //Funció que inicialitza node: tauler buit, n_fills = N, valor = 0, guarda memòria pel vector d'apuntadors als fills
	nodeInicial->n_fills = N;
	nodeInicial->fills = malloc(nodeInicial-> n_fills*sizeof(Node*)); //Reservem memòria: vector de dim n_fills, les components del qual son apuntadors a nodes
	inicialitzaTauler(nodeInicial);
	nodeInicial->valor = 0;
}
int determinaColumna(Node *n, int numFill){ //Donat el nombre de fill, determina a quina columna correspon
//numFill és el nombre de fill (si pot tenir 3 fills, fill 1, fill 2 o fill 3)
	int i, j, columnaFinal;
	int columnesLliures[N]; //vector de N posicions del qual n'ocuparem únicament les primeres n_fills
	
	i = 0;
	for (j=0 ; j<N ; j++){
		if(n->tauler[0][j] == BUIT){
			columnesLliures[i] = j;
			i = i+1;
		}
	}
	columnaFinal = columnesLliures[numFill];
	return columnaFinal;
}
int determinaFila(Node *n, int columna){ //Donada la columna a la que es vol tirar, determina la fila
	int fila, i;
	
	if(n->tauler[0][columna] != BUIT){
		printf("(determinaFila): hi ha algun error, intenta tirar a una columna ja plena");
	}
	else{
		for (i=N-1 ; i>=0 ; i--){
			if(n->tauler[i][columna] == BUIT){
				fila = i;
				break;
			}
		}
	}

	return fila;
}
void aplicaTirada(Node* n, int numFill, char jugador){ //Donat un node pare, aplica la tirada corresponent a numFill. S'utilitza al fer l'arbre
	int columna, fila;
	columna = determinaColumna(n, numFill);
	fila = determinaFila(n, columna);
	n->tauler[fila][columna] = jugador;
}
void ferTirada(Node* n, int columna, char jugador){ //Donada una columna, actualitza el tauler del node amb aquesta tirada. S'utilitza a l'hora de jugar
	int fila;
	fila = determinaFila(n, columna);
	n->tauler[fila][columna] = jugador;
}
int numFills(char tauler[N][N]){ //Compta el nombre de fills que pot tenir un node
	int j, nfills=0;
	for (j=0 ; j<N ; j++){
		if(tauler[0][j] == BUIT){
			nfills = nfills + 1;
		}
	}
	
	return nfills;
}

void pintaTauler (Node n){ //Imprimeix el tauler del node per pantalla
	int i, j;
	printf("\n");
	for (i=0 ; i<N ; i++)
	{ 
		printf("                                   ");
		for ( j=0 ; j<N ; j++)
		{
			printf("|%c", n.tauler[i][j]);
		}
		printf("|\n");
	}
	printf("                                    1 2 3 4 5 6 7 8\n");
}
void copiaTauler(char taulerFill[N][N], char taulerPare[N][N]){ //Copia el tauler del pare al fill
	int i, j;
	for (i=0 ; i<N ; i++){ 
		for (j=0 ; j<N ; j++){
			taulerFill[i][j] = taulerPare[i][j];
		}
	}
}
int taulerPle(char tauler[N][N]){ //Determina si el tauler està ple (retorna 0 si està ple, 1 si no)
	int j, ple = 0;
	for (j=0 ; j<N ; j++){
		if(tauler[0][j] != BUIT){
			ple = ple + 1;
		}
	}
	if(ple == N){return 0;}
	else{return 1;}
}
int columnaPlena(char tauler[N][N], int columnPers){ //Determina si la columna a la qual tira la persona està plena
	if(tauler[0][columnPers] == BUIT){
		return 1;
	}
	else{return 0;}
}

int iniciPartida(Node *node){ //Inicialitza el node i imprimeix per pantalla el tauler i els primers missatges del joc. Deixa escollir amb qui jugar.
	char resposta;
	inicialitzaNode(node);
	
	printf("\nBENVINGUT AL JOC DEL QUATRE EN RATLLA!\n");
	printf("Vols jugar contra l'ordinador o contra una altra persona?\n 1. Contra l'ordinador\n 2. Contra una altra persona\n");
	resposta = getche();
	resposta = (int) resposta -48; //(int) resposta converteix en el nombre ASCII corresponent al caràcter que s'ha entrat. 0 -> 48
	
	while (resposta!=1 && resposta!=2){
		printf ("\nHas triat una opcio que no es valida.\n");
		resposta = getche();
		resposta = (int) resposta -48;
	}
	
	if (resposta==1) {
		printf("\nLes teves fitxes seran les %c i les de l'ordinador, les %c.\n", PERS, ORD);
		pintaTauler(*node);
	}
	else{
		printf("\nJugador 1, les teves fitxes seran les %c.\nJugador 2, les teves fitxes seran les %c.\n", PERS, ORD);
		pintaTauler(*node);
	}
	
	return resposta;
}
int tiradaPersona(int columnPers, Node *node, int jugador){ //Demana a la persona que esculli una columna, actualitza i pinta tauler, i actualitza n_fills
	char columnPers_c;

	printf("\nEscull el nombre de la columna a la qual vols tirar: ");
	columnPers_c = getche(); //fa com scan, però només agafa UN valor
	columnPers = (int) columnPers_c; //(int) columnPers_c converteix en un int (fent que sigui el nombre ASCII corresponent)
		
	while (columnPers < 49 || columnPers > 56 || columnaPlena(node->tauler, columnPers-48-1)==0 ){ //48 valor ASCII de 0, 49-1,..., 56-8
		printf("\nLa columna que has seleccionat no es valida o esta plena, torna a provar.");
		printf("\nEscull el nombre de la columna a la qual vols tirar: ");
		columnPers_c = getche();
		columnPers = (int) columnPers_c;
	}
	columnPers = columnPers - 48 - 1; //Restem 1 perquè la columna que el jugador anomena 1 sigui la nostra 0
	ferTirada(node, columnPers, jugador);
	pintaTauler(*node);
	node->n_fills = numFills(node->tauler); //Actualitzo n_fills
	return columnPers;
}
int quatreRatllaHoritz(char tauler[N][N]){ //Diu si s'ha donat 4 en ratlla horitzontal
	int i, j, qratlla, parar = 0;
	
	for (i=N-1 ; i>=0 && !parar; i--){ //files
		qratlla = 0; //Cada cop que se salta de línia el comptador qratlla torna a 0
		for (j=0 ; j<N && !parar; j++){ //columnes
			if(tauler[i][j] == PERS){
				if(tauler[i][j-1] != PERS || j == 0){ //Si l'anterior no és PERS o si és principi de fila
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[i][j] == ORD){
				if(tauler[i][j-1] != ORD || j == 0){ //Si l'anterior no és ORD o si és principi de fila
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
	}
	return qratlla;
}
int quatreRatllaVert(char tauler[N][N]){ //Diu si s'ha donat un 4 en ratlla vertical
	int i, j, qratlla, parar = 0;
	
	for (j=0 ; j<N && !parar; j++){ //columnes
		qratlla = 0; //Cada cop que se salta de columna el comptador qratlla torna a 0
		for (i=N-1 ; i>=0 && !parar; i--){ //files
			if(tauler[i][j] == PERS){
				if(tauler[i+1][j] != PERS || i == N-1){ //Si l'anterior no és PERS o si és principi de columna
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[i][j] == ORD){
				if(tauler[i+1][j] != ORD || i == N-1){ //Si l'anterior no és ORD o si és principi de columna
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
	}
	return qratlla;
}
int quatreRatllaDiagEsqInf(char tauler[N][N]){ //Diu si hi ha 4 ratlla en diagonal, d'esquerra a dreta, a la diagonal INFERIOR (diag inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	
	for (j=0 ; j<N-3 && !parar; j++){ // j compta les diagonals (va fins N-3 perquè a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=j; i<N && !parar; i++){			
			if(tauler[k+7-i][i] == PERS){ // Es recorr primer tota la diagonal que comença en [7][0], després la que comença en [7][1]... fins la de [7][4]
				if(tauler[k+7-i+1][i-1] != PERS || (k+7-i) == 7){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[k+7-i][i] == ORD){
				if(tauler[k+7-i+1][i-1] != ORD || (k+7-i) == 7){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k+1;
	}
	return qratlla;
}
int quatreRatllaDiagEsqSup(char tauler[N][N]){ //Diu si hi ha 4 ratlla en diagonal, d'esquerra a dreta, a la diagonal SUPERIOR (diag NO inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	
	for (j=N-2; j>2 && !parar; j--){ // j compta les diagonals (va fins 3 perquè a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[6-k-i][i] == PERS){ // Es recorr primer tota la diagonal que comença en [6][0], després la que comença en [5][0]... fins la de [3][0]
				if(tauler[6-k-i+1][i-1] != PERS || i == 0){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[6-k-i][i] == ORD){
				if(tauler[6-k-i+1][i-1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k+1;
	}
	return qratlla;
}
int quatreRatllaDiagDretaInf(char tauler[N][N]){ //Diu si hi ha 4 ratlla en diagonal, de dreta a esquerra, a la diagonal INFERIOR (diag inclosa)
	int i, j, qratlla, parar = 0, k = 4;
	
	for (j=3; j<N && !parar; j++){ // j compta les diagonals (comença a 3 perquè a les primeres diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[7-i][7-k-i] == PERS){ // Es recorr primer tota la diagonal que comença en [7][3], després la que comença en [7][4]... fins la de [7][7]
				if(tauler[7-i+1][7-k-i+1] != PERS || i == 0){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[7-i][7-k-i] == ORD){
				if(tauler[7-i+1][7-k-i+1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k-1; // k = 4, 3, 2, 1, 0
	}
	return qratlla;
}
int quatreRatllaDiagDretaSup(char tauler[N][N]){ //Diu si hi ha 4 ratlla en diagonal, de dreta a esquerra, a la diag SUPERIOR (diag NO inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	
	for (j=6; j>2 && !parar; j--){ // j compta les diagonals (acaba a j = 3 pq a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[6-k-i][7-i] == PERS){ // Es recorr primer tota la diagonal que comença en [6][7], després la que comença en [5][7]... fins la de [3][7]
				if(tauler[6-k-i+1][7-i+1] != PERS || i == 0){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 4){
						parar = 1;
					}
				}
			}
			else if(tauler[6-k-i][7-i] == ORD){
				if(tauler[6-k-i+1][7-i+1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -4){
						parar = -1;
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k+1; // k = 0, 1, 2, 3
	}
	return qratlla;
}
int quatreRatllaDiag(char tauler[N][N]){ //Diu si hi ha 4 ratlla en qualsevol diagonal (retorna 4 si l'ha fet PERS, -4 ORD, 1 si ningú)
	int diag1, diag2, diag3, diag4;
	diag1 = quatreRatllaDiagEsqInf(tauler);
	diag2 = quatreRatllaDiagEsqSup(tauler);
	diag3 = quatreRatllaDiagDretaInf(tauler);
	diag4 = quatreRatllaDiagDretaSup(tauler);
	if(diag1==4 || diag2==4 || diag3==4 || diag4==4){return 4;}
	else if(diag1==-4 || diag2==-4 || diag3==-4 || diag4==-4){return -4;}
	else{return 1;}
}
int quatreRatlla(char tauler[N][N]){ //Retorna 4 si ha fet quatre en ratlla PERS, -4 ORD, 1 si ningú
	int horitz, vert, diag;
	horitz = quatreRatllaHoritz(tauler);
	vert = quatreRatllaVert(tauler);
	diag = quatreRatllaDiag(tauler);
	if(horitz==4 || vert==4 || diag==4){return 4;}
	else if(horitz==-4 || vert==-4 || diag==-4){return -4;}
	else{return 1;}
}
int partidaAcabada(char tauler[N][N], int resposta){ //Diu si la partida s'ha acabat (return 0) -si hi ha 4 ratlla o tauler ple- o si no (return 1)
	if(quatreRatlla(tauler) == 4){
		if(resposta == 1){ //Es juga contra l'ordinador
			printf("\n\nENHORABONA! Has guanyat la partida.");
		}
		else{ //Es juga una persona contra una altra
			printf("\n\nENHORABONA! El jugador %c ha guanyat la partida.", PERS);
		}
		return 0;
	}
	else if(quatreRatlla(tauler) == -4){
		if(resposta == 1){ //Es juga contra l'ordinador
			printf("\n\nLlastima! L\'ordinador ha guanyat la partida.");
		}
		else{ //Es juga una persona contra l'altra
			printf("\n\nENHORABONA! El jugador %c ha guanyat la partida.", ORD);
		}
		return 0;
	}
	else if(taulerPle(tauler) == 0){
		printf("\n\nEMPAT! Ja no queden espais buits al tauler.\n");
		return 0;
	}
	else{return 1;}
}

int determinaRatlla(int r5_p, int r5_o, int r4_p, int r4_o, int r3_p, int r3_o, int r2_p, int r2_o, int r1_p, int r1_o){//Determina el 2,3 ratlla més important
	if(r5_p == 5){return r5_p;}
	else if(r4_p == 4){return r4_p;}
	else if(r5_o == -5){return r5_o;}
	else if(r4_o == -4){return r4_o;}
	else if(r3_p == 3){return r3_p;}
	else if(r2_p == 2){return r2_p;}
	else if(r3_o == -3){return r3_o;}
	else if(r2_o == -2){return r2_o;}
	else if(r1_p == 1){return r1_p;}
	else if(r1_o == -1){return r1_o;}
	else{return 0;}
}
int nRatllaHoritz(char tauler[N][N]){ //Diu si s'ha donat tres o dos en ratlla (amb buits en diferentes posicions) en horitzontal
	int i, j, qratlla, parar = 0;
	// Variables per a determinar si hi ha simultàniament diverses coses de PERS i de ORD:
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5) 
	int r4_p = 0, r4_o = 0; // 2 ratlla + BUIT + 1 ratlla (4)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	int r2_p = 0, r2_o = 0; // BUIT + 1 ratlla + BUIT + 1 ratlla (2)
	int r1_p = 0, r1_o = 0; // 1 ratlla + 2 BUIT + 1 ratlla (1)
	
	for (i=N-1 ; i>=0 && !parar; i--){ //files
		qratlla = 0; //Cada cop que se salta de línia el comptador qratlla torna a 0
		for (j=0 ; j<N && !parar; j++){ //columnes
			if(tauler[i][j] == PERS){
				if(tauler[i][j-1] != PERS || j == 0){ //Si l'anterior no és PERS o si és principi de fila
					qratlla = 1;
					if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT)){ //Si no final de fila, el següent és buit "x _" i baix no és _
						if(j+1!=N-1 && tauler[i][j+2] == PERS && j+2!=N-1 && tauler[i][j+3] == BUIT && (i==N-1 || tauler[i+1][j+3]!=BUIT)){ // Si "x _ x _"
							r2_p = 2;
						}
						else if(j+1!=N-1 && tauler[i][j+2] == PERS && j!=0 && tauler[i][j-1] == BUIT && (i==N-1 || tauler[i+1][j-1]!=BUIT)){ // Si "_ x _ x"
							r2_p = 2;
						}
						else if(j+1!=N-1 && tauler[i][j+2] == BUIT && (i==N-1 || tauler[i+1][j+2]!=BUIT) && j+2!=N-1 && tauler[i][j+3] == PERS){ // Si "x _ _ x"
							r1_p = 1;
						}
					}
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(j == N-1){ // Si està a final de fila
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT)){ //Si el següent està buit
							r5_p = 5; // Hi ha hagut 3 en ratlla + buit
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else if(j-2!=0 && tauler[i][j-3] == BUIT && (i==N-1 || tauler[i+1][j-3]!=BUIT)){
							r5_p = 5; // Hi ha hagut buit + 3 en ratlla
							parar = 1; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT) && j+1!=N-1 && tauler[i][j+2] == PERS){ // Si "x x _ x"
						// Si no està a final de fila i el següent és buit "x x _" i el de baix no, i el seg és "x"
							r4_p = 4;
						}
						else if(j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT) && j-2!=0 && tauler[i][j-3] == PERS){ // Si "x _ x x"
						//Si el primer "x" no és ppi de fila "_ x x" i si l'anterior al buit és "x"
							r4_p = 4;
						}
						else if(j!=N-1 &&tauler[i][j+1]==BUIT &&(i==N-1||tauler[i+1][j+1]!=BUIT) && j+1!=N-1 && tauler[i][j+2]==BUIT &&(i==N-1 || tauler[i+1][j+2]!=BUIT)){ 
						// Si "x x _ _"
							r3_p = 3;
						}
						else if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT) && j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT)){
						// Si "_ x x _"
							r3_p = 3;
						}
						else if(j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT)){ //Si el primer "x" no és ppi de fila "_ x x"
							if(j-2!=0 && tauler[i][j-3] == BUIT && (i==N-1 || tauler[i+1][j-3]!=BUIT)){ // Si "_ _ x x"
								r3_p = 3;
							}
						}
					}
				}
			}
			else if(tauler[i][j] == ORD){
				if(tauler[i][j-1] != ORD || j == 0){ //Si l'anterior no és ORD o si és principi de fila
					qratlla = -1;
					if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT)){ // Si no està a final de fila i el següent és buit "o _"
						if(j+1!=N-1 && tauler[i][j+2] == ORD && j+2!=N-1 && tauler[i][j+3] == BUIT && (i==N-1 || tauler[i+1][j+3]!=BUIT)){ // Si "o _ o _"
							r2_o = -2;
						}
						else if(j+1!=N-1 && tauler[i][j+2] == ORD && j!= 0 && tauler[i][j-1] == BUIT && (i==N-1 || tauler[i+1][j-1]!=BUIT)){ // Si "_ o _ o"
							r2_o = -2;
						}
						else if(j+1!=N-1 && tauler[i][j+2] == BUIT && (i==N-1 || tauler[i+1][j+2]!=BUIT) && j+2!=N-1 && tauler[i][j+3] == ORD){ // Si "o _ _ o"
							r1_o = -1;
						}
					}
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de ORD
						if(j == N-1){ // Si està a final de fila
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT)){ //Si el següent està buit
							r5_o = -5; // Hi ha hagut 3 en ratlla + buit
							qratlla = 0; //Reinicialitzem i seguim buscant si hi ha un n_ratlla millor
						}
						else if(j-2!=0 && tauler[i][j-3] == BUIT && (i==N-1 || tauler[i+1][j-3]!=BUIT)){
							r5_o = -5; // Hi ha hagut buit + 3 en ratlla
							qratlla = 0;
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT) && j+1!=N-1 && tauler[i][j+2] == ORD){ // Si "o o _ o"
						// Si no està a final de fila i el següent és buit "o o _" i el de baix no, i el seg és "o"
							r4_o = -4;
						}
						else if(j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT) && j-2!=0 && tauler[i][j-3] == ORD){ // Si "o _ o o"
						//Si el primer "o" no és ppi de fila "_ o o" i si l'anterior al buit és "o"
							r4_o = -4;
						}
						else if(j!=N-1 &&tauler[i][j+1]==BUIT &&(i==N-1||tauler[i+1][j+1]!=BUIT) && j+1!=N-1 && tauler[i][j+2]==BUIT &&(i==N-1 || tauler[i+1][j+2]!=BUIT)){ 
						// Si "o o _ _"
							r3_o = -3;
						}
						else if(j!=N-1 && tauler[i][j+1] == BUIT && (i==N-1 || tauler[i+1][j+1]!=BUIT) && j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT)){
						// Si "_ o o _"
							r3_o = -3;
						}
						else if(j-1!=0 && tauler[i][j-2] == BUIT && (i==N-1 || tauler[i+1][j-2]!=BUIT)){ //Si el primer "o" no és ppi de fila "_ o o"
							if(j-2!=0 && tauler[i][j-3] == BUIT && (i==N-1 || tauler[i+1][j-3]!=BUIT)){ // Si "_ _ o o"
								r3_o = -3;
							}
						}
					}
				}
			}
			else {qratlla = 0;}
		}
	}
	return determinaRatlla(r5_p, r5_o, r4_p, r4_o, r3_p, r3_o, r2_p, r2_o, r1_p, r1_o);
}
int nRatllaVert(char tauler[N][N]){ //Diu si s'ha donat tres o dos en ratlla (amb buits en diferentes posicions) en vertical
	int i, j, qratlla, parar = 0;
	// Variables per a determinar si hi ha simultàniament diverses coses de PERS i de ORD:
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	
	for (j=0 ; j<N && !parar; j++){ //columnes
		qratlla = 0; //Cada cop que se salta de columna el comptador qratlla torna a 0
		for (i=N-1 ; i>=0 && !parar; i--){ //files
			if(tauler[i][j] == PERS){
				if(tauler[i+1][j] != PERS || i == N-1){ //Si l'anterior no és PERS o si és principi de columna
					qratlla = 1; // Comencem a comptar per a PERS
				}
				else{ //Si l'anterior és persona
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(i == 0 || tauler[i-1][j] != BUIT){ // Si està a final de columna o el següent NO és buit
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else{ //Si el següent està buit I no és final de columna
							r5_p = 5; // Hi ha hagut 3 en ratlla + buit
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(i!=0 && tauler[i-1][j] == BUIT && i-1!=0){ // Si no és final de columna i el següent és buit (i no final), hi ha "x x _" i per tant "x x _ _"
							r3_p = 3;
						}
					}
				}
			}
			else if(tauler[i][j] == ORD){
				if(tauler[i+1][j] != ORD || i == N-1){ //Si l'anterior no és ORD o si és principi de columna
					qratlla = -1;
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de ORD
						if(i == 0 || tauler[i-1][j] != BUIT){ // Si està a final de columna o el següent NO és buit
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else{ //Si el següent està buit I no és final de columna
							r5_o = -5; // Hi ha hagut 3 en ratlla + buit
							qratlla = 0; //Tornem a incialitzar qratlla
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(i!=0 && tauler[i-1][j] == BUIT && i-1!=0){ // Si no està a final de columna i el següent és buit (no final), hi ha "o o _" i per tant "o o _ _"
							r3_o = -3;
						}
					}
				}
			}
			else {qratlla = 0;}
		}
	}
	return determinaRatlla(r5_p, r5_o, 0, 0, r3_p, r3_o, 0, 0, 0, 0);
}
int nRatllaDiagEsqInf(char tauler[N][N]){ //Diu si s'ha donat 3 o 2 ratlla (amb buits) en diagonal, d'esquerra a dreta, a la diagonal INFERIOR (diag inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5) 
	int r4_p = 0, r4_o = 0; // 2 ratlla + BUIT + 1 ratlla (4)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	int r2_p = 0, r2_o = 0; // BUIT + 1 ratlla + BUIT + 1 ratlla (2)
	int r1_p = 0, r1_o = 0; // 1 ratlla + 2 BUIT + 1 ratlla (1)
	
	for (j=0 ; j<N-3 && !parar; j++){ // j compta les diagonals (va fins N-3 perquè a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=j; i<N && !parar; i++){			
			if(tauler[k+7-i][i] == PERS){ // Es recorr primer tota la diagonal que comença en [7][0], després la que comença en [7][1]... fins la de [7][4]
				if(tauler[k+7-i+1][i-1] != PERS || (k+7-i) == 7){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
					if(i!=N-1 && tauler[k+7-i-1][i+1]==BUIT && tauler[k+7-i-1+1][i+1]!=BUIT){//Si no final diag, el següent és buit "x _" i baix no _
						if(i+1!=N-1 && tauler[k+7-i-2][i+2]==PERS && i+2!=N-1 && tauler[k+7-i-3][i+3]==BUIT && tauler[k+7-i-3+1][i+3]!=BUIT){ //"x _ x _"
							r2_p = 2;
						}
						else if(i+1!=N-1 &&tauler[k+7-i-2][i+2]==PERS && i!=j &&tauler[k+7-i+1][i-1]==BUIT &&(i-1==j || tauler[k+7-i+2][i-1]!=BUIT)){//"_ x _ x"
							r2_p = 2;
						}
						else if(i+1!=N-1 && tauler[k+7-i-2][i+2]==BUIT && tauler[k+7-i-2+1][i+2]!=BUIT && i+2!=N-1 && tauler[k+7-i-3][i+3]==PERS){//"x _ _ x"
							r1_p = 1;
						}
					}
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(i == N-1){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT){ //Si el següent està buit
							r5_p = 5; // Hi ha hagut 3 en ratlla + buit
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else if(i-2!=j && tauler[k+7-i+3][i-3]==BUIT && (i-3==j || tauler[k+7-i+4][i-3]!=BUIT)){
							r5_p = 5; // Hi ha hagut buit + 3 en ratlla
							parar = 1; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(i!=N-1 && tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT && i+1!=N-1 && tauler[k+7-i-2][i+2] == PERS){ // Si "x x _ x"
						// Si no està a final de fila i el següent és buit "x x _" i el seg és "x"
							r4_p = 4;
						}
						else if(i-1!=j && tauler[k+7-i+2][i-2]==BUIT && i-2!=j && tauler[k+7-i+2+1][i-2]!=BUIT && tauler[k+7-i+3][i-3] == PERS){ // Si "x _ x x"
						//Si el primer "x" no és ppi de fila "_ x x" i l'anterior al buit és "x"
							r4_p = 4;
						}	
						else if(i!=N-1 &&tauler[k+7-i-1][i+1]==BUIT &&tauler[k+7-i-1+1][i+1]!=BUIT && i+1!=N-1 && tauler[k+7-i-2][i+2]==BUIT &&tauler[k+7-i-2+1][i+2]!=BUIT){
						// Si "x x _ _"
							r3_p = 3;
						}
						else if(i!=N-1 && tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT && i-1!=j && tauler[k+7-i+2][i-2]==BUIT &&(i-2!=j||tauler[k+7-i+2+1][i-2]!=BUIT)){
						// Si "x x _" i l'anterior és buit "_ x x _"
							r3_p = 3;
						}
						else if(i-1!=j && tauler[k+7-i+2][i-2]==BUIT && i-2!=j && tauler[k+7-i+2+1][i-2]!=BUIT){ //Si el primer "x" no és ppi de fila "_ x x"
							if(tauler[k+7-i+3][i-3] == BUIT && (i-3==j || tauler[k+7-i+4][i-3]!=BUIT)){ // Si "_ _ x x"
								r3_p = 3;
							}
						}
					}
				}
			}
			else if(tauler[k+7-i][i] == ORD){
				if(tauler[k+7-i+1][i-1] != ORD || (k+7-i) == 7){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
					if(i!=N-1 && tauler[k+7-i-1][i+1]==BUIT && tauler[k+7-i-1+1][i+1]!=BUIT){//Si no final diag, el següent és buit "o _" i baix no _
						if(i-1!=N-1 && tauler[k+7-i-2][i+2]==ORD && i-2!=N-1 && tauler[k+7-i-3][i+3]==BUIT && tauler[k+7-i-3+1][i+3]!=BUIT){ //"o _ o _"
							r2_o = -2;
						}
						else if(i-1!=N-1 &&tauler[k+7-i-2][i+2]==ORD && i!=j &&tauler[k+7-i+1][i-1]==BUIT &&(i-1==j || tauler[k+7-i+2][i-1]!=BUIT)){//"_ o _ o"
							r2_o = -2;
						}
						else if(i-1!=N-1 && tauler[k+7-i-2][i+2]==BUIT && tauler[k+7-i-2+1][i+2]!=BUIT && i-2!=N-1 && tauler[k+7-i-3][i+3]==ORD){//"o _ _ o"
							r1_o = -1;
						}
					}
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de ORD
						if(i == N-1){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT){ //Si el següent està buit
							r5_o = -5; // Hi ha hagut 3 en ratlla + buit
							qratlla = 0; //Reinicialitzem i seguim buscant si hi ha un n_ratlla millor
						}
						else if(i-2!=j && tauler[k+7-i+3][i-3]==BUIT && (i-3==j || tauler[k+7-i+4][i-3]!=BUIT)){
							r5_o = -5; // Hi ha hagut buit + 3 en ratlla
							qratlla = 0;
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(i!=N-1 && tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT && i+1!=N-1 && tauler[k+7-i-2][i+2] == ORD){ // Si "o o _ o"
						// Si no està a final de fila i el següent és buit "o o _" i el seg és "o"
							r4_o = -4;
						}
						else if(i-1!=j && tauler[k+7-i+2][i-2]==BUIT && i-2!=j && tauler[k+7-i+2+1][i-2]!=BUIT && tauler[k+7-i+3][i-3] == ORD){ // Si "o _ o o"
						//Si el primer "o" no és ppi de fila "_ o o" i l'anterior al buit és "o"
							r4_o = -4;
						}	
						else if(i!=N-1 &&tauler[k+7-i-1][i+1]==BUIT &&tauler[k+7-i-1+1][i+1]!=BUIT && i+1!=N-1 && tauler[k+7-i-2][i+2]==BUIT &&tauler[k+7-i-2+1][i+2]!=BUIT){
						// Si "o o _ _"
							r3_o = -3;
						}
						else if(i!=N-1 && tauler[k+7-i-1][i+1] == BUIT && tauler[k+7-i-1+1][i+1]!=BUIT && i-1!=j && tauler[k+7-i+2][i-2]==BUIT && (i-2!=j||tauler[k+7-i+2+1][i-2]!=BUIT)){
						// Si "o o _" i l'anterior és buit "_ o o _"
							r3_o = -3;
						}
						else if(i-1!=j && tauler[k+7-i+2][i-2]==BUIT && i-2!=j && tauler[k+7-i+2+1][i-2]!=BUIT){ //Si el primer "o" no és ppi de fila "_ o o"
							if(tauler[k+7-i+3][i-3] == BUIT && (i-3==j || tauler[k+7-i+4][i-3]!=BUIT)){ // Si "_ _ o o"
								r3_o = -3;
							}
						}
					} 
				}
			}
			else {qratlla = 0;}
		}
		k = k+1;
	}
	return determinaRatlla(r5_p, r5_o, r4_p, r4_o, r3_p, r3_o, r2_p, r2_o, r1_p, r1_o);
}
int nRatllaDiagEsqSup(char tauler[N][N]){ //Diu si s'ha donat 3 o 2 ratlla (amb buits) en diagonal, d'esquerra a dreta, a la diagonal SUPERIOR (diag NO inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5) 
	int r4_p = 0, r4_o = 0; // 2 ratlla + BUIT + 1 ratlla (4)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	int r2_p = 0, r2_o = 0; // BUIT + 1 ratlla + BUIT + 1 ratlla (2)
	int r1_p = 0, r1_o = 0; // 1 ratlla + 2 BUIT + 1 ratlla (1)
	
	for (j=N-2; j>2 && !parar; j--){ // j compta les diagonals (va fins 3 perquè a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[6-k-i][i] == PERS){ // Es recorr primer tota la diagonal que comença en [6][0], després la que comença en [5][0]... fins la de [3][0]
				if(i == 0 || tauler[6-k-i+1][i-1] != PERS){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
					if(i!=j && tauler[6-k-i-1][i+1]==BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){//Si no final diag, el següent és buit "x _" i baix no _
						if(i+1!=j && tauler[6-k-i-2][i+2]==PERS && i+2!=j && tauler[6-k-i-3][i+3]==BUIT && tauler[6-k-i-3+1][i+3]!=BUIT){ //"x _ x _"
							r2_p = 2;
						}
						else if(i+1!=j &&tauler[6-k-i-2][i+2]==PERS && i!=0 &&tauler[6-k-i+1][i-1]==BUIT &&(i-1==0 || tauler[6-k-i+2][i-1]!=BUIT)){//"_ x _ x"
							r2_p = 2;
						}
						else if(i+1!=j && tauler[6-k-i-2][i+2]==BUIT && tauler[6-k-i-2+1][i+2]!=BUIT && i+2!=j && tauler[6-k-i-3][i+3]==PERS){//"x _ _ x"
							r1_p = 1;
						}
					}
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){ // "x x x _"
							r5_p = 5; 
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else if(i-2!=0 && tauler[6-k-i+3][i-3]==BUIT && (i-3==0 || tauler[6-k-i+4][i-3]!=BUIT)){  // "_ x x x"
							r5_p = 5;
							parar = 1; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT && i+1!=j && tauler[6-k-i-2][i+2] == PERS){// "x x _ x"
						// Si no està a final de diag, el següent és buit "x x _" i el seg no és final de diag i és "x"
							r4_p = 4;							
						}
						else if(i-1!=0 && tauler[6-k-i+2][i-2]==BUIT && tauler[6-k-i+2+1][i-2]!=BUIT && i-2!=0 && tauler[6-k-i+3][i-3] == PERS){// "x _ x x"
						//Si el primer "x" no és ppi de fila "_ x x" --> "x _ x x"
							r4_p = 4;
						}
						else if(i-1!=0 &&tauler[6-k-i+2][i-2]==BUIT &&tauler[6-k-i+3][i-2]!=BUIT &&i-2!=0 &&tauler[6-k-i+3][i-3]==BUIT&&(i-3==0||tauler[6-k-i+4][i-3]!=BUIT)){ 
						// "_ _ x x"
							r3_p = 3;
						}
						else if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT && i-1!=0 && tauler[6-k-i+2][i-2]==BUIT &&(i-2==0||tauler[6-k-i+2+1][i-2]!=BUIT)){
						// Si "x x _" i l'anterior és buit "_ x x _"
							r3_p = 3;
						}
						else if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){ //Si no està a final de fila i el següent és buit "x x _"
							if(i+1!=j && tauler[6-k-i-2][i+2]==BUIT && tauler[6-k-i-2+1][i+2]!=BUIT){ // Si "x x _ _"
								r3_p = 3;
							}
						}
					}
				}
			}
			else if(tauler[6-k-i][i] == ORD){
				if(tauler[6-k-i+1][i-1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
					if(i!=j && tauler[6-k-i-1][i+1]==BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){//Si no final diag, el següent és buit "x _" i baix no _
						if(i+1!=j && tauler[6-k-i-2][i+2]==ORD && i+2!=j && tauler[6-k-i-3][i+3]==BUIT && tauler[6-k-i-3+1][i+3]!=BUIT){ //"x _ x _"
							r2_o = -2;
						}
						else if(i+1!=j &&tauler[6-k-i-2][i+2]==ORD && i!=0 &&tauler[6-k-i+1][i-1]==BUIT &&(i-1==0 || tauler[6-k-i+2][i-1]!=BUIT)){//"_ x _ x"
							r2_o = -2;
						}
						else if(i+1!=j && tauler[6-k-i-2][i+2]==BUIT && tauler[6-k-i-2+1][i+2]!=BUIT && i+2!=j && tauler[6-k-i-3][i+3]==ORD){//"x _ _ x"
							r1_o = -1;
						}
					}
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de PERS "o o o"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){ //Si el següent està buit
							r5_o = -5; // Hi ha hagut 3 en ratlla + buit
							qratlla = 0; //Reinicialitzem i seguim buscant si hi ha un n_ratlla millor
						}
						else if(i-2!=0 && tauler[6-k-i+3][i-3]==BUIT && (i-3==0 || tauler[6-k-i+4][i-3]!=BUIT)){
							r5_o = -5; // Hi ha hagut buit + 3 en ratlla
							qratlla = 0; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT && i+1!=j && tauler[6-k-i-2][i+2] == ORD){// "o o _ o"
						// Si no està a final de diag, el següent és buit "o o _" i el seg no és final de diag i és "o"
							r4_o = -4;							
						}
						else if(i-1!=0 && tauler[6-k-i+2][i-2]==BUIT && i-2!=0 && tauler[6-k-i+2+1][i-2]!=BUIT && tauler[6-k-i+3][i-3] == ORD){// "o _ o o"
						//Si el primer "o" no és ppi de fila "_ o o" --> "o _ o o"
							r4_o = -4;
						}
						else if(i-1!=0 &&tauler[6-k-i+2][i-2]==BUIT &&i-2!=0 &&tauler[6-k-i+3][i-2]!=BUIT &&tauler[6-k-i+3][i-3]==BUIT&&(i-3==0||tauler[6-k-i+4][i-3]!=BUIT)){ 
						// "_ _ o o"
							r3_o = -3;
						}
						else if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT && i-1!=0 && tauler[6-k-i+2][i-2]==BUIT &&(i-2==0||tauler[6-k-i+2+1][i-2]!=BUIT)){
						// Si "x x _" i l'anterior és buit "_ x x _"
							r3_o = -3;
						}
						else if(i!=j && tauler[6-k-i-1][i+1] == BUIT && tauler[6-k-i-1+1][i+1]!=BUIT){ //Si no està a final de fila i el següent és buit "o o _"
							if(i+1!=j && tauler[6-k-i-2][i+2]==BUIT && tauler[6-k-i-2+1][i+2]!=BUIT){ // Si "o o _ _"
								r3_o = -3;
							}
						}
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k+1;
	}
	return determinaRatlla(r5_p, r5_o, r4_p, r4_o, r3_p, r3_o, r2_p, r2_o, r1_p, r1_o);
}
int nRatllaDiagDretaInf(char tauler[N][N]){ //Diu si s'ha donat 3 o 2 ratlla (amb buits) en diagonal, de dreta a esquerra, a la diagonal INFERIOR (diag inclosa)
	int i, j, qratlla, parar = 0, k = 4;
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5) 
	int r4_p = 0, r4_o = 0; // 2 ratlla + BUIT + 1 ratlla (4)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	int r2_p = 0, r2_o = 0; // BUIT + 1 ratlla + BUIT + 1 ratlla (2)
	int r1_p = 0, r1_o = 0; // 1 ratlla + 2 BUIT + 1 ratlla (1)
	
	for (j=3; j<N && !parar; j++){ // j compta les diagonals (comença a 3 perquè a les primeres diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[7-i][7-k-i] == PERS){ // Es recorr primer tota la diagonal que comença en [7][3], després la que comença en [7][4]... fins la de [7][7]
				if(tauler[7-i+1][7-k-i+1] != PERS || i == 0){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
					if(i!=j && tauler[7-i-1][7-k-i-1]==BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){//Si no final diag, el següent és buit "x _" i baix no _
						if(i+1!=j && tauler[7-i-2][7-k-i-2]==PERS && i+2!=j && tauler[7-i-3][7-k-i-3]==BUIT && tauler[7-i-3+1][7-k-i-3]!=BUIT){ //"x _ x _"
							r2_p = 2;
						}
						else if(i+1!=j &&tauler[7-i-2][7-k-i-2]==PERS && i!=0 &&tauler[7-i+1][7-k-i+1]==BUIT &&(i-1==0 || tauler[7-i+2][7-k-i+1]!=BUIT)){//"_ x _ x"
							r2_p = 2;
						}
						else if(i+1!=j && tauler[7-i-2][7-k-i-2]==BUIT && tauler[7-i-2+1][7-k-i-2]!=BUIT && i+2!=j && tauler[7-i-3][7-k-i-3]==PERS){//"x _ _ x"
							r1_p = 1;
						}
					}
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){ // "x x x _"
							r5_p = 5; 
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else if(i-2!=0 && tauler[7-i+3][7-k-i+3]==BUIT && (i-3==0 || tauler[7-i+4][7-k-i+3]!=BUIT)){  // "_ x x x"
							r5_p = 5;
							parar = 1; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT && i+1!=j && tauler[7-i-2][7-k-i-2] == PERS){// "x x _ x"
						// Si no està a final de diag, el següent és buit "x x _" i el seg no és final de diag i és "x"
							r4_p = 4;							
						}
						else if(i-1!=0 && tauler[7-i+2][7-k-i+2]==BUIT && i-2!=0 && tauler[7-i+2+1][7-k-i+2]!=BUIT && tauler[7-i+3][7-k-i+3] == PERS){// "x _ x x"
						//Si el primer "x" no és ppi de fila "_ x x" --> "x _ x x"
							r4_p = 4;
						}
						else if(i-1!=0 &&tauler[7-i+2][7-k-i+2]==BUIT &&i-2!=0 &&tauler[7-i+3][7-k-i+2]!=BUIT &&tauler[7-i+3][7-k-i+3]==BUIT&&(i-3==0||tauler[7-i+4][7-k-i+3]!=BUIT)){ 
						// "_ _ x x"
							r3_p = 3;
						}
						else if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT &&i-1!=0 &&tauler[7-i+2][7-k-i+2]==BUIT &&(i-2!=0||tauler[7-i+3][7-k-i+2]!=BUIT)){
						// Si "x x _" i l'anterior és buit "_ x x _"
							r3_p = 3;
						}
						else if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){ //Si no està a final de fila i el següent és buit "x x _"
							if(i+1!=j && tauler[7-i-2][7-k-i-2]==BUIT && tauler[7-i-2+1][7-k-i-2]!=BUIT){ // Si "x x _ _"
								r3_p = 3;
							}
						}
					}
				}
			}
			else if(tauler[7-i][7-k-i] == ORD){
				if(tauler[7-i+1][7-k-i+1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
					if(i!=j && tauler[7-i-1][7-k-i-1]==BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){//Si no final diag, el següent és buit "o _" i baix no _
						if(i+1!=j && tauler[7-i-2][7-k-i-2]==ORD && i+2!=j && tauler[7-i-3][7-k-i-3]==BUIT && tauler[7-i-3+1][7-k-i-3]!=BUIT){ //"o _ o _"
							r2_o = -2;
						}
						else if(i+1!=j &&tauler[7-i-2][7-k-i-2]==ORD && i!=0 &&tauler[7-i+1][7-k-i+1]==BUIT &&(i-1==0 || tauler[7-i+2][7-k-i+1]!=BUIT)){//"_ o _ o"
							r2_o = -2;
						}
						else if(i+1!=j && tauler[7-i-2][7-k-i-2]==BUIT && tauler[7-i-2+1][7-k-i-2]!=BUIT && i+2!=j && tauler[7-i-3][7-k-i-3]==ORD){//"o _ _ o"
							r1_o = -1;
						}
					}
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de PERS "o o o"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){ // "o o o _"
							r5_o = -5; 
							qratlla = 0; //Reinicialitzem i seguim buscant si hi ha un n_ratlla millor
						}
						else if(i-2!=0 && tauler[7-i+3][7-k-i+3]==BUIT && (i-3==0 || tauler[7-i+4][7-k-i+3]!=BUIT)){  // "_ o o o"
							r5_o = -5;
							qratlla = 0; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT && i+1!=j && tauler[7-i-2][7-k-i-2] == ORD){// "o o _ o"
						// Si no està a final de diag, el següent és buit "o o _" i el seg no és final de diag i és "o"
							r4_o = -4;							
						}
						else if(i-1!=0 && tauler[7-i+2][7-k-i+2]==BUIT && i-2!=0 && tauler[7-i+2+1][7-k-i+2]!=BUIT && tauler[7-i+3][7-k-i+3] == ORD){// "o _ o o"
						//Si el primer "o" no és ppi de fila "_ o o" --> "o _ o o"
							r4_o = -4;
						}
						else if(i-1!=0 &&tauler[7-i+2][7-k-i+2]==BUIT &&i-2!=0 &&tauler[7-i+3][7-k-i+2]!=BUIT &&tauler[7-i+3][7-k-i+3]==BUIT&&(i-3==0||tauler[7-i+4][7-k-i+3]!=BUIT)){ 
						// "_ _ o o"
							r3_o = -3;
						}
						else if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT &&i-1!=0 &&tauler[7-i+2][7-k-i+2]==BUIT &&(i-2!=0||tauler[7-i+3][7-k-i+2]!=BUIT)){
						// Si "o o _" i l'anterior és buit "_ o o _"
							r3_o = -3;
						}
						else if(i!=j && tauler[7-i-1][7-k-i-1] == BUIT && tauler[7-i-1+1][7-k-i-1]!=BUIT){ //Si no està a final de fila i el següent és buit "o o _"
							if(i+1!=j && tauler[7-i-2][7-k-i-2]==BUIT && tauler[7-i-2+1][7-k-i-2]!=BUIT){ // Si "o o _ _"
								r3_o = -3;
							}
						}
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k-1; // k = 4, 3, 2, 1, 0
	}
	return determinaRatlla(r5_p, r5_o, r4_p, r4_o, r3_p, r3_o, r2_p, r2_o, r1_p, r1_o);
}
int nRatllaDiagDretaSup(char tauler[N][N]){ //Diu si s'ha donat 3 o 2 ratlla (amb buits) en diagonal, de dreta a esquerra, a la diag SUPERIOR (diag NO inclosa)
	int i, j, qratlla, parar = 0, k = 0;
	int r5_p = 0, r5_o = 0; // 3 ratlla + BUIT (5) 
	int r4_p = 0, r4_o = 0; // 2 ratlla + BUIT + 1 ratlla (4)
	int r3_p = 0, r3_o = 0; // BUIT + 2 ratlla + BUIT (3)
	int r2_p = 0, r2_o = 0; // BUIT + 1 ratlla + BUIT + 1 ratlla (2)
	int r1_p = 0, r1_o = 0; // 1 ratlla + 2 BUIT + 1 ratlla (1)
	
	for (j=6; j>2 && !parar; j--){ // j compta les diagonals (acaba a j = 3 pq a les últimes diagonals (de 3, 2 i 1 caselles) no hi pot haver 4 en ratlla)
		qratlla = 0; // Cada cop que fem una nova diagonal s'inicialitza qratlla
		for (i=0; i<=j && !parar; i++){		
			if(tauler[6-k-i][7-i] == PERS){ // Es recorr primer tota la diagonal que comença en [6][7], després la que comença en [5][7]... fins la de [3][7]
				if(tauler[6-k-i+1][7-i+1] != PERS || i == 0){ //Si l'anterior no és PERS o si és principi de diagonal
					qratlla = 1;
					if(i!=j && tauler[6-k-i-1][7-i-1]==BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){//Si no final diag, el següent és buit "x _" i baix no _
						if(i+1!=j && tauler[6-k-i-2][7-i-2]==PERS && i+2!=j && tauler[6-k-i-3][7-i-3]==BUIT && tauler[6-k-i-3+1][7-i-3]!=BUIT){ //"x _ x _"
							r2_p = 2;
						}
						else if(i+1!=j &&tauler[6-k-i-2][7-i-2]==PERS && i!=0 &&tauler[6-k-i+1][7-i+1]==BUIT &&(i-1==0 || tauler[6-k-i+2][7-i+1]!=BUIT)){//"_ x _ x"
							r2_p = 2;
						}
						else if(i+1!=j && tauler[6-k-i-2][7-i-2]==BUIT && tauler[6-k-i-2+1][7-i-2]!=BUIT && i+2!=j && tauler[6-k-i-3][7-i-3]==PERS){//"x _ _ x"
							r1_p = 1;
						}
					}
				}
				else{
					qratlla = qratlla + 1;
					if(qratlla == 3){ // Si hi ha 3 en ratlla de PERS "x x x"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){ // "x x x _"
							r5_p = 5; 
							parar = 1; //Si hi ha hagut 3 en ratlla de PERS és el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else if(i-2!=0 && tauler[6-k-i+3][7-i+3]==BUIT && (i-3==0 || tauler[6-k-i+4][7-i+3]!=BUIT)){  // "_ x x x"
							r5_p = 5;
							parar = 1; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == 2){ // Si hi ha "x x"
						if(i!=j && tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT && i+1!=j && tauler[6-k-i-2][7-i-2] == PERS){// "x x _ x"
						// Si no està a final de diag, el següent és buit "x x _" i el seg no és final de diag i és "x"
							r4_p = 4;							
						}
						else if(i-1!=0 && tauler[6-k-i+2][7-i+2]==BUIT && i-2!=0 && tauler[6-k-i+2+1][7-i+2]!=BUIT && tauler[6-k-i+3][7-i+3] == PERS){// "x _ x x"
						//Si el primer "x" no és ppi de fila "_ x x" --> "x _ x x"
							r4_p = 4;
						}
						else if(i-1!=0 &&tauler[6-k-i+2][7-i+2]==BUIT &&i-2!=0 &&tauler[6-k-i+3][7-i+2]!=BUIT &&tauler[6-k-i+3][7-i+3]==BUIT&&(i-3==0||tauler[6-k-i+4][7-i+3]!=BUIT)){ 
						// "_ _ x x"
							r3_p = 3;
						}
						else if(i-1!=0 && tauler[6-k-i+2][7-i+2]==BUIT &&tauler[6-k-i+2+1][7-i+2]!=BUIT && i-1!=0 &&tauler[6-k-i+2][7-i+2]==BUIT &&(i-2!=0||tauler[6-k-i+3][7-i+2]!=BUIT)){
						// Si "x x _" i l'anterior és buit "_ x x _"
							r3_p = 3;
						}
						else if(i!=j && tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){ //Si no està a final de fila i el següent és buit "x x _"
							if(i+1!=j && tauler[6-k-i-2][7-i-2]==BUIT && tauler[6-k-i-2+1][7-i-2]!=BUIT){ // Si "x x _ _"
								r3_p = 3;
							}
						}
					}
				}
			}
			else if(tauler[6-k-i][7-i] == ORD){
				if(tauler[6-k-i+1][7-i+1] != ORD || i == 0){ //Si l'anterior no és ORD o si és principi de diagonal
					qratlla = -1;
					if(i!=j && tauler[6-k-i-1][7-i-1]==BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){//Si no final diag, el següent és buit "o _" i baix no _
						if(i+1!=j && tauler[6-k-i-2][7-i-2]==ORD && i+2!=j && tauler[6-k-i-3][7-i-3]==BUIT && tauler[6-k-i-3+1][7-i-3]!=BUIT){ //"o _ o _"
							r2_o = -2;
						}
						else if(i+1!=j &&tauler[6-k-i-2][7-i-2]==ORD && i!=0 &&tauler[6-k-i+1][7-i+1]==BUIT &&(i-1==0 || tauler[6-k-i+2][7-i+1]!=BUIT)){//"_ o _ o"
							r2_o = -2;
						}
						else if(i+1!=j && tauler[6-k-i-2][7-i-2]==BUIT && tauler[6-k-i-2+1][7-i-2]!=BUIT && i+2!=j && tauler[6-k-i-3][7-i-3]==ORD){//"o _ _ o"
							r1_o = -1;
						}
					}
				}
				else{
					qratlla = qratlla - 1;
					if(qratlla == -3){ // Si hi ha 3 en ratlla de PERS "o o o"
						if(i == j){ // Si està a final de diag
							qratlla = 0; // No és 3 en ratlla bo, així que reinicialitzem i aquest no el tenim en compte
						}
						else if(tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){ // "o o o _"
							r5_o = -5; 
							qratlla = 0; //Reinicialitzem i seguim buscant si hi ha un n_ratlla millor
						}
						else if(i-2!=0 && tauler[6-k-i+3][7-i+3]==BUIT && (i-3==0 || tauler[6-k-i+4][7-i+3]!=BUIT)){  // "_ o o o"
							r5_o = -5;
							qratlla = 0; //És el màxim que podem tenir, de manera que no cal seguir buscant
						}
						else{ 
							qratlla = 0; // Si cap dels anteriors es compleix no és un 3 ratlla vàlid
						}
					}
					else if(qratlla == -2){ // Si hi ha "o o"
						if(i!=j && tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT && i+1!=j && tauler[6-k-i-2][7-i-2] == ORD){// "o o _ o"
						// Si no està a final de diag, el següent és buit "o o _" i el seg no és final de diag i és "o"
							r4_o = -4;							
						}
						else if(i-1!=0 && tauler[6-k-i+2][7-i+2]==BUIT && i-2!=0 && tauler[6-k-i+2+1][7-i+2]!=BUIT && tauler[6-k-i+3][7-i+3] == ORD){// "o _ o o"
						//Si el primer "o" no és ppi de fila "_ o o" --> "o _ o o"
							r4_o = -4;
						}
						else if(i-1!=0 &&tauler[6-k-i+2][7-i+2]==BUIT &&i-2!=0 &&tauler[6-k-i+3][7-i+2]!=BUIT &&tauler[6-k-i+3][7-i+3]==BUIT&&(i-3==0||tauler[6-k-i+4][7-i+3]!=BUIT)){ 
						// "_ _ o o"
							r3_o = -3;
						}
						else if(i-1!=0 && tauler[6-k-i+2][7-i+2]==BUIT &&tauler[6-k-i+2+1][7-i+2]!=BUIT && i-1!=0 &&tauler[6-k-i+2][7-i+2]==BUIT &&(i-2!=0||tauler[6-k-i+3][7-i+2]!=BUIT)){
						// Si "o o _" i l'anterior és buit "_ o o _"
							r3_o = -3;
						}
						else if(i!=j && tauler[6-k-i-1][7-i-1] == BUIT && tauler[6-k-i-1+1][7-i-1]!=BUIT){ //Si no està a final de fila i el següent és buit "o o _"
							if(i+1!=j && tauler[6-k-i-2][7-i-2]==BUIT && tauler[6-k-i-2+1][7-i-2]!=BUIT){ // Si "o o _ _"
								r3_o = -3;
							}
						}
					}
				}
			}
			else {qratlla = 0;}
		}
		k = k+1; // k = 0, 1, 2, 3
	}
	return determinaRatlla(r5_p, r5_o, r4_p, r4_o, r3_p, r3_o, r2_p, r2_o, r1_p, r1_o);
}
int nRatllaDiag(char tauler[N][N]){ //Retorna el 3 o 2 ratlla més important en qualsevol diagonal (retorna + si l'ha fet PERS, - ORD, 1 si ningú)
	int diag1, diag2, diag3, diag4;
	diag1 = nRatllaDiagEsqInf(tauler);
	diag2 = nRatllaDiagEsqSup(tauler);
	diag3 = nRatllaDiagDretaInf(tauler);
	diag4 = nRatllaDiagDretaSup(tauler);
	if(diag1==5 || diag2==5 || diag3==5 || diag4==5){return 5;}
	else if(diag1==4 || diag2==4 || diag3==4 || diag4==4){return 4;}
	else if(diag1==-5 || diag2==-5 || diag3==-5 || diag4==-5){return -5;}
	else if(diag1==-4 || diag2==-4 || diag3==-4 || diag4==-4){return -4;}
	else if(diag1==3 || diag2==3 || diag3==3 || diag4==3){return 3;}
	else if(diag1==2 || diag2==2 || diag3==2 || diag4==2){return 2;}
	else if(diag1==-3 || diag2==-3 || diag3==-3 || diag4==-3){return -3;}
	else if(diag1==-2 || diag2==-2 || diag3==-2 || diag4==-2){return -2;}
	else if(diag1==1 || diag2==1 || diag3==1 || diag4==1){return 1;}
	else if(diag1==-1 || diag2==-1 || diag3==-1 || diag4==-1){return -1;}
	else{return 0;}
}
int nRatlla(char tauler[N][N]){ //Retorna el millor 3 o 2 ratlla de PERS, ORD o 0 si no hi ha res
	int horitz, vert, diag;
	horitz = nRatllaHoritz(tauler);
	vert = nRatllaVert(tauler);
	diag = nRatllaDiag(tauler);
	if(horitz==5 || vert==5 || diag==5){return 5;}
	else if(horitz==4 || vert==4 || diag==4){return 4;}
	else if(horitz==-5 || vert==-5 || diag==-5){return -5;}
	else if(horitz==-4 || vert==-4 || diag==-4){return -4;}
	else if(horitz==3 || vert==3 || diag==3){return 3;}
	else if(horitz==2 || vert==2 || diag==2){return 2;}
	else if(horitz==-3 || vert==-3 || diag==-3){return -3;}
	else if(horitz==-2 || vert==-2 || diag==-2){return -2;}
	else if(horitz==1 || vert==1 || diag==1){return 1;}
	else if(horitz==-1 || vert==-1 || diag==-1){return -1;}
	else{return 0;}
}


int main(){
	int columnPers = 0, columnaOrd, resposta;
	Node *node = malloc(sizeof(Node)); //Creo un apuntador que apunti a un Node i reservo la memòria per aquest Node (com a la funció creaNode)
	resposta = iniciPartida(node);
	if(resposta == 1){
		while(partidaAcabada(node->tauler, resposta) != 0){
			printf("\nEs el teu torn.");
			columnPers = tiradaPersona(columnPers, node, PERS);
			if(partidaAcabada(node->tauler, resposta) == 0){
				break;
			}
			else{
				columnaOrd = miniMax(node);
				ferTirada(node, columnaOrd, ORD);
				pintaTauler(*node);
				printf("\nL\'ordinador ha tirat a la columna %d.", columnaOrd+1);
			}
		}		
	}
	else{
		while(partidaAcabada(node->tauler, resposta) != 0){
			printf("\nJugador 1, es el teu torn.");
			columnPers = tiradaPersona(columnPers, node, PERS);
			printf("\nEl jugador 1 ha tirat a la columna %d.", columnPers+1);
			if(partidaAcabada(node->tauler, resposta) == 0){
				break;
			}
			else{
				printf("\nJugador 2, es el teu torn.");
				columnPers = tiradaPersona(columnPers, node, ORD);
				printf("\nEl jugador 2 ha tirat a la columna %d.", columnPers+1);
			}
		}
	}
	
	esborraNode(node);
	return 0;
}
