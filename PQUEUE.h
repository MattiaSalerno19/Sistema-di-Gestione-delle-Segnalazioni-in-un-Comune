typedef struct c_PQ *PQueue; //Struttura della coda

typedef struct Data; //Struttura per la data

typedef struct segnalazione; //Struttura per la segnalazione


//Prototipi delle funzioni
PQueue newPQ(void);
int emptyPQ(PQueue q);
int insert (PQueue q, segnalazione key);
int delete(PQueue q, int cod_id);