typedef struct c_PQ *PQueue; //Struttura della coda

typedef struct Data; //Struttura per la data

typedef struct segnalazione; //Struttura per la segnalazione


//Prototipi delle funzioni
PQueue newPQ(void);
int emptyPQ(PQueue q);
int insert (PQueue q, segnalazione key);
int delete(PQueue q, int cod_id);
void visualizzaTutte(PQueue q);
void visualizzaPerStato(PQueue q, int stato_cercato);
void visualizzaPerUrgenza(PQueue q, int urgenza_cercata);
void visualizzaPerCodice(PQueue q, int codice_cercato);
void visualizzaPerCategoria(PQueue q, int categoria_cercata);
int rendi_chiudibile(PQueue q, int cod_id);
int cambia_stato(PQueue q, int cod_id, int nuovo_stato);   
