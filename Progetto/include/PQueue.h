typedef struct c_PQ *PQueue; //Struttura della coda

typedef struct data Data;
typedef struct segnalazione Segnalazione;


//Prototipi delle funzioni
Segnalazione newSegnalazione(int cod_id, const char *nome, const char *cogn,int cat, const char *desc,int giorno, int mese, int anno,int urg, int stato, int rimuovibile);
PQueue newPQ(void);
int emptyPQ(PQueue q);
void DestroyPQueue(PQueue q);
int insert (PQueue q, Segnalazione key);
int delete(PQueue q, int cod_id);
int confronta_segnalazioni(const void *a, const void *b);
void visualizzaTutte(PQueue q);
void visualizzaPerStato(PQueue q, int stato_cercato);
void visualizzaPerUrgenza(PQueue q, int urgenza_cercata);
void visualizzaPiuUrgenti(PQueue q, int i);
void visualizzaPerCodice(PQueue q, int codice_cercato);
void visualizzaPerCategoria(PQueue q, int categoria_cercata);
int rendi_rimovibile(PQueue q, int cod_id);
int cambia_stato(PQueue q, int cod_id, int nuovo_stato);
int caricaDaFile(PQueue q, const char *nomefile);
int insertSegnalazione(PQueue q, int cod_id, const char *nome, const char *cogn,int cat, const char *desc,int giorno, int mese, int anno,int urg, int stato);
void stampa_report(PQueue q);
