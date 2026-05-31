#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include "PQueue.h" 
#include "hash.h"
#define MAXPQ 150  // Definizione della dimensione massima dell'heap.

typedef struct data { //struttura per la data.
    int giorno;
    int mese;
    int anno;
} Data;

typedef struct segnalazione{ //Struttura per la segnalazione
    int cod_id; //Codice identificativo della segnalazione
    char cit_nome[20]; //Nome del cittadino che ha effettuato la segnalazione
    char cit_cogn[20]; //Cognome del cittadino che ha effettuato la segnalazione
    int cat; //Categoria del problema che varierà da 1 a 5
    char desc[150]; //Descrizione del problema
    Data data; //Data della segnalazione
    int urg; //Livello di urgenza
    int stato; //Stato della segnalazione
    int rimuovibile; //Flag per indicare se la segnalazione è rimovibile (1) o meno (0)
} Segnalazione;

// Definizione della struttura per la coda di priorità.
struct c_PQ {
    Segnalazione vet[MAXPQ+1];  // Array per memorizzare gli elementi dell'heap, 
    int numel;       // Numero attuale di elementi nell'heap.
    hashtable hash; // Tabella hash per mappare i codici identificativi alle posizioni nell'heap.
};


// Dichiarazione delle funzioni statiche per la manipolazione dell'heap.
static int scendi (PQueue q, int i);
static int sali (PQueue q, int pos);

//Funzione di confronto per ordinare le segnalazioni in base alla data
// Ritorna un valore > 0 se d1 è successiva a d2 (più recente)
// Ritorna un valore < 0 se d1 è precedente a d2 (più vecchia)
// Ritorna 0 se sono uguali
static int confronta_date(Data d1, Data d2) {
    if (d1.anno != d2.anno) 
        return d1.anno - d2.anno;
    if (d1.mese != d2.mese) 
        return d1.mese - d2.mese;
    return d1.giorno - d2.giorno;
}

// Funzione costruttore per una segnalazione.
Segnalazione newSegnalazione(int cod_id, const char *nome, const char *cogn,int cat, const char *desc,int giorno, int mese, int anno,int urg, int stato, int rimuovibile) {
    Segnalazione s;
    s.cod_id      = cod_id;
    s.cat         = cat;
    s.urg         = urg;
    s.stato       = stato;
    s.rimuovibile = rimuovibile;
    s.data.giorno = giorno;
    s.data.mese   = mese;
    s.data.anno   = anno;
    strncpy(s.cit_nome, nome, sizeof(s.cit_nome) - 1);
    s.cit_nome[sizeof(s.cit_nome) - 1] = '\0';
    strncpy(s.cit_cogn, cogn, sizeof(s.cit_cogn) - 1);
    s.cit_cogn[sizeof(s.cit_cogn) - 1] = '\0';
    strncpy(s.desc, desc, sizeof(s.desc) - 1);
    s.desc[sizeof(s.desc) - 1] = '\0';
    return s;
}

// Stampa la descrizione sostituendo '_' con spazio per la leggibilità. Le descrizioni hanno '_' al posto degli spazi per facilitare la lettura da file.
static void stampa_desc(const char *desc) {
    for (int i = 0; desc[i] != '\0'; i++)
        putchar(desc[i] == '_' ? ' ' : desc[i]);
    putchar('\n');
}


PQueue newPQ(void)
{
    PQueue q;
    q = malloc(sizeof(struct c_PQ));  // Allocazione della memoria per la coda.
    if (q == NULL) return NULL;       
    q->numel = 0; 
    q->hash = newHashtable(MAXPQ * 2); 
    if (q->hash == NULL) {
        free(q); // Se fallisce l'hash, libera la memoria allocata per la coda e ritorna NULL.
        return NULL;
    }                  
    return q;                        
}

// Funzione per controllare se la coda è vuota.
int emptyPQ(PQueue q)
{
     if (!q) return 1;            // Controlla se il puntatore è NULL.
     return q->numel == 0;        // Ritorna vero se la coda è vuota.
}

//Funzione di stampa per una singola segnalazione
static void stampa_singola(Segnalazione s){
    printf("ID: %-5d | Urgenza: %-2d | Stato: %-10d | Data: %02d/%02d/%d\n",s.cod_id, s.urg, s.stato, s.data.giorno, s.data.mese, s.data.anno);
    printf("Cittadino: %s %s | Categoria: %d\n", s.cit_nome, s.cit_cogn, s.cat);
    printf("Descrizione: ");
    stampa_desc(s.desc);
    printf("==================================================================\n");
}

// Funzione per riaggiustare l'heap verso il basso.
static int scendi(PQueue q, int i)
{
    Segnalazione temp;
    int n = q->numel, pos;

    while (1) // Determina la posizione del figlio con il valore più grande.
    {
        
        if (2*i + 1 <= n){  // Se il nodo corrente ha due figli.
            if((q->vet[2*i].urg > q->vet[2*i + 1].urg || (q->vet[2*i].urg == q->vet[2*i + 1].urg && confronta_date(q->vet[2*i].data, q->vet[2*i + 1].data) < 0))){
                pos = 2*i;  // Il figlio sinistro ha priorità maggiore.
            }
            else{   
                pos = 2*i + 1;  // Il figlio destro ha priorità maggiore.
            }
        }
      
        else if(2*i <= n){  // Se il nodo corrente ha solo un figlio.
            pos = 2*i;
        }
       
        else{
            break;  // Se non ha figli, interrompe il ciclo.
        }
       
        // Scambia i valori se il figlio ha un valore più grande.
        if (q->vet[pos].urg > q->vet[i].urg || (q->vet[pos].urg == q->vet[i].urg && confronta_date(q->vet[pos].data, q->vet[i].data) < 0))   
        {
            temp = q->vet[i];
            q->vet[i] = q->vet[pos];
            q->vet[pos] = temp;
            // Aggiorna le posizioni nella tabella hash dopo lo scambio.
            //Controlla il corretto inserimento dell'hash, ritorna 0 in caso di errore annulando i cambiamenti.
            if (!hash_insert(q->hash, q->vet[i].cod_id, i)) {
                q->vet[pos] = q->vet[i];
                q->vet[i] = temp;
                return 0;
            }
            if (!hash_insert(q->hash, q->vet[pos].cod_id, pos)) {
                // Ripristina l'array
                q->vet[pos] = q->vet[i];
                q->vet[i] = temp;
                hash_insert(q->hash, q->vet[pos].cod_id, pos);
                hash_insert(q->hash, q->vet[i].cod_id, i);
                return 0;
            }
            i = pos;  // Continua a scendere nell'heap.
        }
        else{
           break;}  // Termina se non sono necessari altri scambi.
    }
    return 1;
}

// Funzione per riaggiustare l'heap verso l'alto.
static int sali(PQueue q, int pos)
{
    Segnalazione temp;
    int i = pos / 2;

    while (pos > 1)
    {
        // Se l'elemento inserito è maggiore del suo genitore, scambia i loro valori.
        if (q->vet[pos].urg > q->vet[i].urg || (q->vet[pos].urg == q->vet[i].urg && confronta_date(q->vet[pos].data, q->vet[i].data) < 0))
        {
            temp = q->vet[i];
            q->vet[i] = q->vet[pos];
            q->vet[pos] = temp;
            // Aggiorna le posizioni nella tabella hash dopo lo scambio.
            //Controlla il corretto inserimento dell'hash, ritorna 0 in caso di errore annulando i cambiamenti.

            if (!hash_insert(q->hash, q->vet[i].cod_id, i)) {
                q->vet[pos] = q->vet[i];
                q->vet[i] = temp;
                return 0;
            }
            if (!hash_insert(q->hash, q->vet[pos].cod_id, pos)) {
                // Ripristina l'array
                q->vet[pos] = q->vet[i];
                q->vet[i] = temp;
                hash_insert(q->hash, q->vet[pos].cod_id, pos);
                hash_insert(q->hash, q->vet[i].cod_id, i);
                return 0;
            }
            pos = i;
            i = pos / 2;  // Continua a salire nell'heap.
        }
        else{
            break;  // Termina se non sono necessari altri scambi.
        }
    }
    return 1;
}

//Funzione per distruggere la coda di priorità e liberare la memoria allocata.
void DestroyPQueue(PQueue q) {
    if (q == NULL) return;
    
    //Distruggo la tabella.
    DestroyHashtable(q->hash);
    
    //Libero la memoria allocata per l'heap.
    free(q);
}

// Funzione per inserire un nuovo elemento nell'heap.
int insert(PQueue q, Segnalazione key)
{
     if (!q || q->numel == MAXPQ) return 0;  // Verifica se la coda è piena o il puntatore è NULL.

     q->numel++;  // Incrementa il numero di elementi.
     q->vet[q->numel] = key;  // Inserisce il nuovo elemento all'ultima posizione.
     if (!hash_insert(q->hash, key.cod_id, q->numel)) { // Inserisce la posizione dell'elemento nella tabella hash.
        q->numel--;  // Annulla l'inserimento in caso di errore.
        printf("Errore di inserimento nella tabella hash.\n");
        return 0;
     }

     if (!sali(q, q->numel)) {  // Riaggiusta l'heap risalendo l'elemento inserito, se fallisce lo elimina e ritorna errore.
         hashDelete(q->hash, key.cod_id);
         q->numel--;
         return 0;
     }
     return 1; 
}

//Funzione per eliminare un elemento dato il suo id
int delete(PQueue q, int cod_id) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da eliminare.\n");
        return 0;
    }
    
    int i = hashSearch(q->hash, cod_id);
    if (i == -1) {
        printf("Elemento non trovato.\n");
        return 0;
    }

    if (q->vet[i].rimuovibile == 0) {
        printf("La segnalazione non è rimovibile.\n");
        return 0;
    }

    // Cancello il vecchio record dall'hash prima di alterare l'heap
    hashDelete(q->hash, cod_id);

    if (i == q->numel) {
        // Se era l'ultimo elemento decremento il contatore.
        q->numel--;
        return 1;
    } 
    else {
        Segnalazione removed_element = q->vet[i]; //Metto da parte l'elemento rimosso in caso di errore.
        q->vet[i] = q->vet[q->numel]; //Sposto l'ultimo elemento nella posizione dell'elemento da eliminare.
        q->numel--;  

        // Aggiorno la posizione dell'elemento spostato all'interno dell'hash.
        if (!hash_insert(q->hash, q->vet[i].cod_id, i)) {
            q->numel++;                        
            q->vet[q->numel] = q->vet[i];      
            q->vet[i] = removed_element;       

            // Provo a rimettere il vecchio elemento nell'hash
            if (!hash_insert(q->hash, removed_element.cod_id, i)) {
            printf("\n Errore critico\n");
            return 2; //In caso fallisca l'aggiornamento di posizione e poi fallisca il rollback, ritorna errore critico per poi chiudere il programma.
        }
        
        return 0; // Fallimento standard: rollback riuscito, l'albero è salvo
        }
        
        int padre = i / 2;
        if (i > 1 && (q->vet[i].urg > q->vet[padre].urg || (q->vet[i].urg == q->vet[padre].urg && confronta_date(q->vet[i].data, q->vet[padre].data) < 0))) {
            if (!sali(q, i)) {
                printf("Errore interno in sali: eliminazione annullata.\n");
                return 0;
            }
        }
        else {
            if (!scendi(q, i)) {
                printf("Errore interno in scendi: eliminazione annullata.\n");
                return 0;
            }
        }
        return 1;
    }

    
}

// Funzione di confronto per qsort.
int confronta_segnalazioni(const void *a, const void *b) {
    Segnalazione *s1 = (Segnalazione *)a;
    Segnalazione *s2 = (Segnalazione *)b;
    
    if (s2->urg != s1->urg){ //Paragona l'urgenza.
        return s2->urg - s1->urg;
    }    
    return confronta_date(s1->data, s2->data);
}

// Funzione per visionare tutte le segnalazioni.
void visualizzaTutte(PQueue q) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }

    Segnalazione *vet_temp = malloc(q->numel * sizeof(Segnalazione)); //Alloco un array temporaneo.
    if (!vet_temp) {
        printf("Errore di allocazione della memoria.\n");
        return;
    }

    memcpy(vet_temp, &q->vet[1], q->numel * sizeof(Segnalazione)); //Funzione di copia da string.h per copiare blocchi di memoria.

    qsort(vet_temp, q->numel, sizeof(Segnalazione), confronta_segnalazioni);//Funzione di ordinamento da stlib.h.

    printf("\n--- TUTTE LE SEGNALAZIONI ---\n");
    for (int i = 0; i < q->numel; i++) {
        stampa_singola(vet_temp[i]);
    }

    free(vet_temp);//Libero la memoria allocata per l'array temporaneo.
}

//Funzione per visionare le segnalazioni in base allo stato.
void visualizzaPerStato(PQueue q, int stato_cercato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    int trovate = 0;
    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].stato == stato_cercato) {
            stampa_singola(q->vet[i]);
            trovate++;
        }
    }
    if (trovate == 0) printf("Nessuna segnalazione con questo stato.\n");
}

//Funzione per visionare le segnalazioni in base al livello di urgenza.
void visualizzaPerUrgenza(PQueue q, int urgenza_cercata) {  

    int trovate = 0;
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return; 
    }
       
    printf("\n--- SEGNALAZIONI CON URGENZA %d ---\n", urgenza_cercata);
    
    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].urg == urgenza_cercata) {
            stampa_singola(q->vet[i]);
            trovate++;
        }
    }

    if (trovate == 0) printf("Nessuna segnalazione con questa urgenza.\n");
}

//Funzione per visionare le segnalazioni tramite codice identificativo.
void visualizzaPerCodice(PQueue q, int codice_cercato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return; 
    }
    
    int i = hashSearch(q->hash, codice_cercato);
    if (i != -1) {
        printf("\nSegnalazione trovata:\n");
        stampa_singola(q->vet[i]);
    } 
    else {
        printf("\nNessuna segnalazione trovata con il codice %d.\n", codice_cercato);
    }

}

//Funzione per visionare le segnalazioni tramite categoria.
void visualizzaPerCategoria(PQueue q, int categoria_cercata) {
    int trovate = 0;

    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }

    printf("\n--- SEGNALAZIONI CATEGORIA %d ---\n", categoria_cercata);

    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cat == categoria_cercata) {
            stampa_singola(q->vet[i]);
            trovate++;
        }
    }

    if (trovate == 0) {
        printf("Nessuna segnalazione trovata per la categoria %d.\n", categoria_cercata);
    }   
}

//Funzione per rendere una segnalazione chiudibile tramite codice id.
int rendi_chiudibile(PQueue q, int cod_id) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da rendere chiudibile.\n");
        return 0;
    }

    int i = hashSearch(q->hash, cod_id);
    if (i != -1) {
        if (q->vet[i].rimuovibile == 1) {
            printf("Segnalazione già rimovibile.\n");
            return 0; 
        }
        q->vet[i].rimuovibile = 1; 
        return 1; 
    }
    
    printf("Segnalazione non trovata.\n");
    return 0;
}

// Carica le segnalazioni da un file txt nella coda di priorità.
//I campi sono separati da spazi, seguono l'ordine con cui newsegnalazione le riceve, la descrizion al posto degli spazi usa '_'
int caricaDaFile(PQueue q, const char *nomefile) {
    FILE *f = fopen(nomefile, "r");
    if (f == NULL) {
        printf("Errore: impossibile aprire il file '%s'.\n", nomefile);
        return 0;
    }

    int cod_id, cat, giorno, mese, anno, urg, stato, rimuovibile;
    char nome[20], cogn[20], desc[150];
    int caricate = 0;

    while (fscanf(f, "%d %19s %19s %d %149s %d %d %d %d %d %d",
                  &cod_id, nome, cogn, &cat, desc,
                  &giorno, &mese, &anno,
                  &urg, &stato, &rimuovibile) == 11) {

        Segnalazione s = newSegnalazione(cod_id, nome, cogn, cat, desc,
                                         giorno, mese, anno,
                                         urg, stato, rimuovibile);
        if (!insert(q, s))
            printf("Attenzione: segnalazione id=%d non inserita.\n", cod_id);
        else
            caricate++;
    }

    fclose(f);
    printf("%d segnalazioni caricate da '%s'.\n", caricate, nomefile);
    return caricate;
}

//Funzione per cambiare lo stato di una segnalazione tramite codice id.
int cambia_stato(PQueue q, int cod_id, int nuovo_stato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da modificare.\n");
        return 0; // Esce se non ci sono segnalazioni.
    }

    int i = hashSearch(q->hash, cod_id);
    if (i != -1) {
        printf("Stato attuale: %d\n", q->vet[i].stato);
        q->vet[i].stato = nuovo_stato; 
        printf("Nuovo stato: %d\n", q->vet[i].stato);
        return 1; 
    }

    printf("Segnalazione non trovata.\n");
    return 0; // Ritorna 0 se la segnalazione non viene trovata.
}
