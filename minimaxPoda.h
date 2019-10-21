typedef struct node{ //Definim Node, el nou tipus d'estructura que emprarem
	struct node* *fills; //Vector d'apuntadors que apunten al node
	int n_fills; //Número de fills que pot tenir aquest node
	char tauler[N][N];
	double valor; //Funció heurística
} Node;

/*Declaració de les funcions*/
Node* creaNode(Node *pare, int numFill, int nivell, char jugador);
void esborraNode(Node *n);
void crea1Nivell(Node* pare, int nivell, char jugador);
void esborraNivell(Node *pare);
void creaArbre(Node* arrel, int k);
void esborraArbre(Node* pare);
void recorreArbre(Node *pare, int k);
int funcioHeuristica(Node* n, int jugadaPropera);
int min(double a, double b);
int max(double a, double b);
int ompleMinMax(Node* pare, int jugadaPropera, double beta);
int miniMax(Node* pare);