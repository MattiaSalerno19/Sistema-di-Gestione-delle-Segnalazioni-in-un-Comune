#include <stdio.h>
#include <stdlib.h>
#include "PQUEUE.h"  // Inclusione dell'header file per la coda di priorità.
#define MAXPQ 150  // Definizione della dimensione massima dell'heap.

// Definizione della struttura per la coda di priorità.
struct c_PQ {
    segnalazione vet[MAXPQ+1];  // Array per memorizzare gli elementi dell'heap, 
    int numel;       // Numero attuale di elementi nell'heap.
};

typedef struct{ //Struttura per la data
    int giorno;
    int mese;
    int anno;
} Data;

typedef struct{ //Struttura per la segnalazione
    int cod_id; //codice identificativo della segnalazione
    char cit_nome[20]; //nome del cittadino che ha effettuato la segnalazione
    char cit_cogn[20]; //cognome del cittadino che ha effettuato la segnalazione
    int cat; //categoria del problema che varierà da 1 a 5
    char desc[150]; //descrizione del problema
    Data data; //data della segnalazione
    int urg; //livello di urgenza
    int stato; //stato della segnalazione
    int rimuovibile; //flag per indicare se la segnalazione è rimovibile (1) o meno (0)
}segnalazione;

// Dichiarazione delle funzioni statiche per la manipolazione dell'heap.
static void scendi (PQueue q, int i);
static void sali (PQueue q);

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

// Funzione per creare una nuova coda di priorità.
PQueue newPQ(void)
{
    PQueue q;
    q = malloc(sizeof(struct c_PQ));  // Allocazione della memoria per la coda.
    if (q == NULL) return NULL;       // Gestione dell'errore di allocazione.
    q->numel = 0;                     // Inizializzazione del numero di elementi a zero.
    return q;                         // Ritorno del puntatore alla coda.
}

// Funzione per controllare se la coda è vuota.
int emptyPQ(PQueue q)
{
     if (!q) return 1;            // Controlla se il puntatore è NULL.
     return q->numel == 0;        // Ritorna vero se la coda è vuota.
}

//Funzione di stampa per una singola segnalazione
static void stampa_singola(segnalazione s){
    printf("ID: %-5d | Urgenza: %-2d | Stato: %-10d | Data: %02d/%02d/%d\n",s.cod_id, s.urg, s.stato, s.data.giorno, s.data.mese, s.data.anno);
    printf("Cittadino: %s %s | Categoria: %d\n", s.cit_nome, s.cit_cogn, s.cat);
    printf("Descrizione: %s\n", s.desc);
    printf("==================================================================\n");
}

// Funzione per riaggiustare l'heap verso il basso.
static void scendi(PQueue q, int i)
{
    segnalazione temp;
    int n = q->numel, pos;
    int controllo_data;

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
             i = pos;  // Continua a scendere nell'heap.
        }
        else{
           break;}  // Termina se non sono necessari altri scambi.
    }
}

// Funzione per riaggiustare l'heap verso l'alto.
static void sali(PQueue q)
{
    segnalazione temp;
    int pos = q->numel, i = pos / 2;

    while (pos > 1)
    {
       // Se l'elemento inserito è maggiore del suo genitore, scambia i loro valori.
       if (q->vet[pos].urg > q->vet[i].urg || (q->vet[pos].urg == q->vet[i].urg && confronta_date(q->vet[pos].data, q->vet[i].data) < 0))
       {
             temp = q->vet[i];
             q->vet[i] = q->vet[pos];
             q->vet[pos] = temp;
             pos = i;
             i = pos / 2;  // Continua a salire nell'heap.
       }
       else
             break;  // Termina se non sono necessari altri scambi.
     }
}

// Funzione per inserire un nuovo elemento nell'heap.
int insert(PQueue q, segnalazione key)
{
     if (!q || q->numel == MAXPQ) return 0;  // Verifica se la coda è piena o il puntatore è NULL.

     q->numel++;  // Incrementa il numero di elementi.
     q->vet[q->numel] = key;  // Inserisce il nuovo elemento all'ultima posizione.

     sali(q);  // Riaggiusta l'heap risalendo l'elemento inserito.
     return 1; // Ritorna 1 per indicare successo.
}

//FUnzione per eliminare un elemento dato il suo id
int delete(PQueue q, int cod_id) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da eliminare.\n");
        return 0; // Esce se non ci sono segnalazioni.
    }
    
   
    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cod_id == cod_id) {  // Trova l'elemento con il codice identificativo specificato.
            
            if (q->vet[i].rimuovibile == 0) return 0; // Se l'elemento non è rimovibile, ritorna 0.

            q->vet[i] = q->vet[q->numel];  // Sostituisce l'elemento da eliminare con l'ultimo elemento.
            q->numel--;  // Decrementa il numero di elementi.

            scendi(q, i);  // Riaggiusta l'heap scendendo l'elemento sostituito.
            return 1; // Ritorna 1 per indicare successo.
        }
        
    }
    return 0; // Ritorna 0 se l'elemento con il codice identificativo specificato non viene trovato.
    
}
