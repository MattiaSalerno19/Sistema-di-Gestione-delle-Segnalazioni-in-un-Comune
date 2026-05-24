#include <stdio.h>
#include <stdlib.h>
#include "PQUEUE.h" 
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
    int cod_id; //Codice identificativo della segnalazione
    char cit_nome[20]; //Nome del cittadino che ha effettuato la segnalazione
    char cit_cogn[20]; //Cognome del cittadino che ha effettuato la segnalazione
    int cat; //Categoria del problema che varierà da 1 a 5
    char desc[150]; //Descrizione del problema
    Data data; //Data della segnalazione
    int urg; //Livello di urgenza
    int stato; //Stato della segnalazione
    int rimuovibile; //Flag per indicare se la segnalazione è rimovibile (1) o meno (0)
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
    q = malloc(sizeof(struct c_PQ));
    if (q == NULL) return NULL;       
    q->numel = 0;                     
    return q;                         
}

// Funzione per controllare se la coda è vuota.
int emptyPQ(PQueue q)
{
     if (!q) return 1;            
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
    return 1; 
}

//Funzione per eliminare un elemento dato il suo id
int delete(PQueue q, int cod_id) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da eliminare.\n");
        return 0;
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

// Funzione per visionare tutte le segnalazioni.
void visualizzaTutte(PQueue q) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    PQueue temp = newPQ(); //Creo una copia temporanea della coda per stampare le segnalazioni in ordine di urgenza
    temp->numel = q->numel;

    for (int i = 1; i <= q->numel; i++) { //Copio gli elementi.
        temp->vet[i] = q->vet[i];
        rendi_chiudibile(temp, temp->vet[i].cod_id); //Rendo tutte le segnalazioni removibili per poterle eliminare dopo la stampa.
    }

    while (temp->numel > 0) {
        stampa_singola(temp->vet[1]); // Stampa la segnalazione con la massima urgenza.
        delete(temp, temp->vet[1].cod_id); // Elimina la segnalazione stampata dalla coda temporanea.
    }
    free(temp); // Libera la memoria allocata per la coda temporanea.

}

//Funzione per visionare le segnalazioni in base allo stato.
void visualizzaPerStato(PQueue q, int stato_cercato) {
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

    if (trovate == 0) {
        printf("Nessuna segnalazione trovata con livello di urgenza %d.\n", urgenza_cercata);
    } 
    else {
        printf("Totale trovate: %d\n", trovate);
    }
}

//Funzione per visionare le segnalazioni tramite codice identificativo.
void visualizzaPerCodice(PQueue q, int codice_cercato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return; 
    }
      

    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cod_id == codice_cercato) {
            printf("\nSegnalazione trovata:\n");
            stampa_singola(q->vet[i]);
            return;
        }
    }

    // Se il ciclo finisce senza entrare nell'if, significa che il codice non esiste.
    printf("\nNessuna segnalazione trovata con il codice %d.\n", codice_cercato);
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

    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cod_id == cod_id) {  // Trova l'elemento con il codice identificativo specificato.
            if (q->vet[i].rimuovibile == 1) {
                printf("Segnalazione già rimovibile.\n");
                return 0; // Se la segnalazione è già rimovibile, ritorna 0.
            }
            q->vet[i].rimuovibile = 1; // Imposta la segnalazione come chiudibile.
            return 1; // Ritorna 1 per indicare successo.
        }
    }
    printf("Segnalazione non trovata.\n");
    return 0; // Ritorna 0 se la segnalazione non viene trovata.
}

//Funzione per cambiare lo stato di una segnalazione tramite codice id.
int cambia_stato(PQueue q, int cod_id, int nuovo_stato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da modificare.\n");
        return 0; // Esce se non ci sono segnalazioni.
    }

    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cod_id == cod_id) {  // Trova l'elemento con il codice identificativo specificato.
            printf("Stato attuale: %d\n", q->vet[i].stato);
            q->vet[i].stato = nuovo_stato; //Cambia lo stato
            printf("Nuovo stato: %d\n", q->vet[i].stato);
            return 1; // Ritorna 1 per indicare successo.
        }
    }
    printf("Segnalazione non trovata.\n");
    return 0; // Ritorna 0 se la segnalazione non viene trovata.
}