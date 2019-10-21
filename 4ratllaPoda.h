void inicialitzaTauler(Node *n);
void inicialitzaNode(Node *nodeInicial);
void pintaTauler (Node n);
int determinaColumna(Node *n, int numFill);
int determinaFila(Node *n, int columna);
void aplicaTirada(Node* n, int numFill, char jugador);
int numFills(char tauler[N][N]);
void copiaTauler(char taulerFill[N][N], char taulerPare[N][N]);
int taulerPle(char tauler[N][N]);
int iniciPartida(Node *node);
int tiradaPersona(int columnPers, Node *node, int jugador);
void missatgeAcabat(int qratlla, int resposta);
int quatreRatllaHoritz(char tauler[N][N]);
int quatreRatllaVert(char tauler[N][N]);
int quatreRatllaDiagEsqInf(char tauler[N][N]);
int quatreRatllaDiagEsqSup(char tauler[N][N]);
int quatreRatllaDiagDretaInf(char tauler[N][N]);
int quatreRatllaDiagDretaSup(char tauler[N][N]);
int quatreRatllaDiag(char tauler[N][N]);
int quatreRatlla(char tauler[N][N]);
int partidaAcabada(char tauler[N][N], int resposta);
int determinaRatlla(int r5_p, int r5_o, int r4_p, int r4_o, int r3_p, int r3_o, int r2_p, int r2_o, int r1_p, int r1_o);
int nRatllaHoritz(char tauler[N][N]);
int nRatllaVert(char tauler[N][N]);
int nRatllaDiagEsqInf(char tauler[N][N]);
int nRatllaDiagEsqSup(char tauler[N][N]);
int nRatllaDiagDretaInf(char tauler[N][N]);
int nRatllaDiagDretaSup(char tauler[N][N]);
int nRatllaDiag(char tauler[N][N]);
int nRatlla(char tauler[N][N]);