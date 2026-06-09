#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include "PQueue.h" 
#include "hash.h"
#define MAXPQ 150  // Definizione della dimensione massima dell'heap.

typedef struct data { //Struttura per la data.
    int giorno;
    int mese;
    int anno;
} Data;

typedef struct segnalazione{ //Struttura per la segnalazione
    int cod_id; //Codice identificativo della segnalazione
    char cit_nome[20]; //Nome del cittadino che ha effettuato la segnalazione
    char cit_cogn[20]; //Cognome del cittadino che ha effettuato la segnalazione
    int cat; //Categoria del problema che varia da 1 a 5
    char desc[150]; //Descrizione del problema
    Data data; //Data della segnalazione
    int urg; //Livello di urgenza
    int stato; //Stato della segnalazione da 1 a 3
    int rimuovibile; //Flag per indicare se la segnalazione è rimovibile (1) o meno (0)
} Segnalazione;

struct nodo_indice{ //Nodo per le liste.
    int cod_id;
    struct nodo_indice *next;
};

//Definizione della struttura per la coda di priorità.
struct c_PQ {
    Segnalazione vet[MAXPQ+1];  //Array per memorizzare gli elementi dell'heap, 
    int numel;       //Numero attuale di elementi nell'heap.
    hashtable hash; //Tabella hash per mappare i codici identificativi alle posizioni nell'heap.
    int max_id; //ID maggiore mai inserito nella coda, necessario per la generazione automatica.
    struct nodo_indice *indice_stato[4];
    struct nodo_indice *indice_urgenza[6];
    struct nodo_indice *indice_categoria[6];
};

//Funzione generica per aggiungere agli indici gli id.
void aggiungi_indice(struct nodo_indice **head, int id){
    struct nodo_indice *new = malloc(sizeof(struct nodo_indice));
    new->cod_id = id;
    new->next = *head;
    *head = new;

}

//Fuznione generica per rimuovere dagli indici gli id.
void rimuovi_indice(struct nodo_indice **head, int id) {
    struct nodo_indice *curr = *head, *prev = NULL;
    while (curr != NULL && curr->cod_id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL) {
        if (prev == NULL) *head = curr->next;
        else prev->next = curr->next;
        free(curr);
    }
}

// Dichiarazione delle funzioni statiche per la manipolazione dell'heap.
static int scendi (PQueue q, int i);
static int sali (PQueue q, int pos);

//Funzione di confronto per ordinare le segnalazioni in base alla data
//Ritorna un valore > 0 se d1 è successiva a d2 (più recente)
//Ritorna un valore < 0 se d1 è precedente a d2 (più vecchia)
//Ritorna 0 se sono uguali
static int confronta_date(Data d1, Data d2) {
    if (d1.anno != d2.anno) 
        return d1.anno - d2.anno;
    if (d1.mese != d2.mese) 
        return d1.mese - d2.mese;
    return d1.giorno - d2.giorno;
    //A parità di urgenza, la priorità andrà alla segnalazione più vecchia.
}

//Funzione di controllo per verificare se un anno è bisestile.
static int bisestile(int anno) {
    return (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);
}

//Funzione di controllo per la validità della data.
static int data_valida(int g, int m, int a) {

    if (a <= 0) return 0; 
    if (m < 1 || m > 12) return 0;
    if (g < 1) return 0;

    int giorni_per_mese[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    
    if (m == 2 && bisestile(a)) {
        giorni_per_mese[2] = 29;
    }

    if (g > giorni_per_mese[m]) return 0;

    return 1;
}

// Funzione per creare una nuova segnalazione.
Segnalazione newSegnalazione(int cod_id, const char *nome, const char *cogn,int cat, const char *desc,int giorno, int mese, int anno,int urg, int stato, int rimuovibile) {
    Segnalazione s;
    //Controllo dei parametri, se uno è errato il codice id diventa -1 e caricaDaFile gestisce l'errore.
    if (cod_id <= 0) {
        printf("Errore: ID %d non valido (deve essere maggiore di 0).\n", cod_id);
        s.cod_id = -1; 
        return s;
    }
    if (cat < 1 || cat > 5) {
        printf("Errore (ID %d): Categoria %d non valida (deve essere tra 1 e 5).\n", cod_id, cat);
        s.cod_id = -1;
        return s;
    }
    if (stato < 1 || stato > 3) {
        printf("Errore (ID %d): Stato %d non valido (deve essere tra 1 e 3).\n", cod_id, stato);
        s.cod_id = -1;
        return s;
    }
    if (rimuovibile != 0 && rimuovibile != 1) {
        printf("Errore (ID %d): Flag rimuovibile %d non valido (deve essere 0 o 1).\n", cod_id, rimuovibile);
        s.cod_id = -1;
        return s;
    }
    if (urg < 1 || urg > 5) {
        printf("Errore (ID %d): Livello urgenza %d non valido (deve essere >= 1).\n", cod_id, urg);
        s.cod_id = -1;
        return s;
    }
    if (!data_valida(giorno, mese, anno)) {
        printf("Errore (ID %d): Data %02d/%02d/%d inesistente o non valida.\n", cod_id, giorno, mese, anno);
        s.cod_id = -1;
        return s;
    }
    
    // Se tutti i parametri sono validi, inizializza la segnalazione.
    s.cod_id = cod_id;
    s.cat = cat;
    s.urg = urg;
    s.stato = stato;
    s.rimuovibile = rimuovibile;
    s.data.giorno = giorno;
    s.data.mese = mese;
    s.data.anno = anno;
    
    //Copia le stringe aggiungendo \0 alla fine.
    strncpy(s.cit_nome, nome, sizeof(s.cit_nome) - 1);
    s.cit_nome[sizeof(s.cit_nome) - 1] = '\0';
    strncpy(s.cit_cogn, cogn, sizeof(s.cit_cogn) - 1);
    s.cit_cogn[sizeof(s.cit_cogn) - 1] = '\0';
    strncpy(s.desc, desc, sizeof(s.desc) - 1);
    s.desc[sizeof(s.desc) - 1] = '\0';
    return s;
}

//Funzione per creare la nuova coda di priorità.
PQueue newPQ(void)
{
    PQueue q;
    q = malloc(sizeof(struct c_PQ));  // Allocazione della memoria per la coda.
    if (q == NULL) return NULL;       
    q->numel = 0; 
    q->hash = newHashtable(MAXPQ * 2); 
    q->max_id = 0;
    if (q->hash == NULL) {
        free(q); // Se fallisce l'hash, libera la memoria allocata per la coda e ritorna NULL.
        return NULL;
    } 
    for(int i=0; i<4; i++){
        q->indice_stato[i] = NULL;
    }
    for(int i=0; i<6; i++){
        q->indice_categoria[i] = NULL;
    }
    for(int i=0; i<6; i++){
        q->indice_urgenza[i] = NULL;               
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
    const char* nome_cat ="Categoria sconosciuta";
    switch (s.cat) {
        case 1: nome_cat = "Manutenzione stradale"; break;
        case 2: nome_cat = "Ambiente e rifiuti"; break;
        case 3: nome_cat = "Impianti pubblici"; break;
        case 4: nome_cat = "Segnaletica e viabilità"; break;
        case 5: nome_cat = "Altro"; break;
    }
    printf("ID: %-5d | Urgenza: %-2d | Stato: %-10d | Data: %02d/%02d/%d\n",s.cod_id, s.urg, s.stato, s.data.giorno, s.data.mese, s.data.anno);
    printf("Cittadino: %s %s | Categoria: %d (%s) \n", s.cit_nome, s.cit_cogn, s.cat, nome_cat);
    printf("Descrizione: %s\n", s.desc);
    printf("==================================================================\n");
}

// Funzione per riaggiustare l'heap verso il basso.
static int scendi(PQueue q, int i)
{
    Segnalazione temp;
    int n = q->numel, pos;

    while (1)
    {      
        if (2*i + 1 <= n){  // Se il nodo corrente ha due figli.
            if((q->vet[2*i].urg > q->vet[2*i + 1].urg || (q->vet[2*i].urg == q->vet[2*i + 1].urg && confronta_date(q->vet[2*i].data, q->vet[2*i + 1].data) < 0))){
                pos = 2*i;  // Il figlio sinistro ha priorità maggiore.
            }
            else{   
                pos = 2*i + 1;  // Il figlio destro ha priorità maggiore o uguale.
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

//Funzione per riaggiustare l'heap verso il basso, senza considerare l'heap.
static int scendi_nohash(PQueue q, int i){
    Segnalazione temp;
    int n = q->numel, pos;

    while (1)
    {      
        if (2*i + 1 <= n){  // Se il nodo corrente ha due figli.
            if((q->vet[2*i].urg > q->vet[2*i + 1].urg || (q->vet[2*i].urg == q->vet[2*i + 1].urg && confronta_date(q->vet[2*i].data, q->vet[2*i + 1].data) < 0))){
                pos = 2*i;  // Il figlio sinistro ha priorità maggiore.
            }
            else{   
                pos = 2*i + 1;  // Il figlio destro ha priorità maggiore o uguale.
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

    //Libero gli indici
    for (int i = 0; i < 4; i++) {
        struct nodo_indice *curr = q->indice_stato[i];
        while (curr != NULL) {
            struct nodo_indice *temp = curr;
            curr = curr->next;
            free(temp); // Libera ogni nodo della lista.
        }
    }
    
    for (int i = 0; i < 6; i++) {
        struct nodo_indice *curr = q->indice_categoria[i];
        while (curr != NULL) {
            struct nodo_indice *temp = curr;
            curr = curr->next;
            free(temp); // Libera ogni nodo della lista.
        }
    }
    
    for (int i = 0; i < 6; i++) {
        struct nodo_indice *curr = q->indice_urgenza[i];
        while (curr != NULL) {
            struct nodo_indice *temp = curr;
            curr = curr->next;
            free(temp); // Libera ogni nodo della lista.
        }
    }
    
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

//Funzione per eliminare un elemento dato il suo id.
int delete(PQueue q, int cod_id) {
    
    if (emptyPQ(q)) { //Se la coda è vuota
        printf("Nessuna segnalazione da eliminare.\n");
        return 0;
    }
    
    int i = hashSearch(q->hash, cod_id); //Uso hashSearch per trovare la posizione dell'elemento senza scorrere l'intera coda.
    
    if (i == -1) { //Se l'elemento non esiste.
        printf("Elemento non trovato.\n");
        return 0;
    }

    if (q->vet[i].rimuovibile == 0) { //Se non è rimovibile.
        printf("La segnalazione non è rimovibile.\n");
        return 0;
    }

    // Cancello il vecchio record dall'hash prima di alterare l'heap.
    hashDelete(q->hash, cod_id);

    rimuovi_indice(&q->indice_stato[q->vet[i].stato], cod_id);
    rimuovi_indice(&q->indice_categoria[q->vet[i].cat], cod_id);
    rimuovi_indice(&q->indice_urgenza[q->vet[i].urg], cod_id);


    if (i == q->numel) { // Se era l'ultimo elemento decremento il contatore.     
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
                printf("\n Errore critico, la struttura dati è danneggiata.Il programma verrà terminato.\n");
                return 2; //In caso fallisca l'aggiornamento di posizione e poi fallisca il rollback, ritorna errore critico per poi chiudere il programma.
            }
            
            printf("Errore interno nella tabella hash: eliminazione annullata.\n");
            return 0; //Fallisce l'aggiornamento di posizione ma funziona il rollback.
        }
        
        int padre = i / 2;
        // Riaggiusta l'elemento spostato.
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

//Funzione per rendere una segnalazione rimovibile tramite codice id.
int rendi_rimovibile(PQueue q, int cod_id) {
    
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione presente.\n");
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

// Funzione per visionare tutte le segnalazioni.
void visualizzaTutte(PQueue q) {
    
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }

    PQueue q_temp = newPQ(); //Creo una coda temporanea
    if (!q_temp) {
        printf("Errore di allocazione della struttura della coda temporanea.\n");
        return;
    }

    q_temp->numel = q->numel; //Sincronizzo il numeri di elementi
    memcpy(&q_temp->vet[1], &q->vet[1], q->numel * sizeof(Segnalazione)); //Funzione di copia da string.h per copiare blocchi di memoria.

    printf("\n--- TUTTE LE SEGNALAZIONI ---\n");
    
    while(q_temp->numel > 0){
        stampa_singola(q_temp->vet[1]); //Stampa la radice, ha priorità massima.
        q_temp->vet[1] = q_temp->vet[q_temp->numel];
        q_temp->numel--;
        if (q_temp->numel > 0) {
            scendi_nohash(q_temp, 1);
        }
    }

    DestroyPQueue(q_temp); //Distrugge la coda temporanea.

}

//Funzione per visionare le segnalazioni in base allo stato.
void visualizzaPerStato(PQueue q, int stato_cercato) {
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    int trovate = 0;
    struct nodo_indice *curr = q->indice_stato[stato_cercato]; //Accede all'indice.

    printf("\n--- SEGNALAZIONI CON STATO %d ---\n", stato_cercato);

    while (curr != NULL) {
        int pos = hashSearch(q->hash, curr->cod_id);//Traduce l'id nella posizione dell'heap.
        
        if (pos != -1) {
            stampa_singola(q->vet[pos]);
            trovate++;
        }
        curr = curr->next;
    }
    
    if (trovate == 0) printf("Nessuna segnalazione con questo stato.\n");
}

//Funzione per visionare le segnalazioni in base al livello di urgenza.
void visualizzaPerUrgenza(PQueue q, int urgenza_cercata) {  
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    int trovate = 0;
    struct nodo_indice *curr = q->indice_urgenza[urgenza_cercata]; //Accede all'indice.

    printf("\n--- SEGNALAZIONI CON URGENZA %d ---\n", urgenza_cercata);

    while (curr != NULL) {
        int pos = hashSearch(q->hash, curr->cod_id);//Traduce l'id nella posizione dell'heap.
        
        if (pos != -1) {
            stampa_singola(q->vet[pos]);
            trovate++;
        }
        curr = curr->next;
    }
    
    if (trovate == 0) printf("Nessuna segnalazione con questa urgenza.\n");
}

//Funzione per visionare le prime "i" segnalazioni più urgenti.
void visualizzaPiuUrgenti(PQueue q, int i){
    
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }

    if (i>q->numel){
        printf("Non ci sono abbastanza segnalazioni, massimo %d \n", q->numel);
        return;
    }
    if (i<1){
        printf("Numero non valido, deve essere maggiore di 0.\n");
        return;
    }

    PQueue q_temp = newPQ(); //Creo una coda temporanea
    if (!q_temp) {
        printf("Errore di allocazione della struttura della coda temporanea.\n");
        return;
    }

    q_temp->numel = q->numel; //Sincronizzo il numeri di elementi
    memcpy(&q_temp->vet[1], &q->vet[1], q->numel * sizeof(Segnalazione)); //Funzione di copia da string.h per copiare blocchi di memoria.

    printf("\n--- PRIME %d SEGNALAZIONI ---\n", i);
    
    for(int t=0;t<i;t++){
        stampa_singola(q_temp->vet[1]); //Stampa la radice, ha priorità massima.
        q_temp->vet[1] = q_temp->vet[q_temp->numel];
        q_temp->numel--;
        if (q_temp->numel > 0) {
            scendi_nohash(q_temp, 1);
        }
    }

    DestroyPQueue(q_temp); //Distrugge la coda temporanea.
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
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    int trovate = 0;
    struct nodo_indice *curr = q->indice_categoria[categoria_cercata]; //Accede all'indice.

    printf("\n--- SEGNALAZIONI CON CATEGORIA %d ---\n", categoria_cercata);
    
    while (curr != NULL) {
        int pos = hashSearch(q->hash, curr->cod_id);//Traduce l'id nella posizione dell'heap.
        
        if (pos != -1) {
            stampa_singola(q->vet[pos]);
            trovate++;
        }
        curr = curr->next;
    }
    
    if (trovate == 0) printf("Nessuna segnalazione con questo stato.\n"); 
}

// Carica le segnalazioni da un file txt nella coda di priorità.
int caricaDaFile(PQueue q, const char *nomefile) {
    FILE *f = fopen(nomefile, "r");
    if (f == NULL) {
        printf("Errore: impossibile aprire il file '%s'.\n", nomefile);
        return 0;
    }

    int cod_id, cat, giorno, mese, anno, urg, stato;
    char nome[20], cogn[20], desc[150];

    char riga[300];
    while (fgets(riga, sizeof(riga), f)) {
        
        int letti = sscanf(riga, "%d %19s %19s %d %d %d %d %d %d  %[^\n]", &cod_id, nome, cogn, &cat, &giorno, &mese, &anno, &urg, &stato, desc);
        if (letti == 10) {
            if (hashSearch(q->hash, cod_id) != -1) {
                printf("Errore: ID %d gia' esistente. Salto la riga...\n", cod_id);
                continue; // Salta questa riga e passa alla prossima
            }
            if (cod_id==0){
                cod_id=((q->max_id)+1);
                q->max_id=cod_id;
                printf("Codice id assegnato:%d\n", cod_id);
            }
            if (cod_id>q->max_id){
            q->max_id=cod_id;
            }
            Segnalazione s = newSegnalazione(cod_id, nome, cogn, cat, desc,giorno, mese, anno,urg, stato, 0);
            if (s.cod_id == -1){
                printf("Segnalazione non salvata.\n");
                continue; // Salta questa segnalazione e continua con la successiva.
            }
            
            if (!insert(q, s)){
                printf("Attenzione: segnalazione id=%d non inserita.\n", cod_id);
                return 0;
            }
            aggiungi_indice(&q->indice_stato[stato], cod_id);
            aggiungi_indice(&q->indice_urgenza[urg], cod_id);
            aggiungi_indice(&q->indice_categoria[cat], cod_id);
        }    
    }

    fclose(f);
    return 1;
}

//Funzione per inserire una nuova segnalazione tramite input da terminale.
//Necessaria per non definire la struttura nel main.
int insertSegnalazione(PQueue q, int cod_id, const char *nome, const char *cogn,int cat, const char *desc,int giorno, int mese, int anno,int urg, int stato) {
    if (hashSearch(q->hash, cod_id) != -1) {
        printf("Errore: ID %d gia' esistente.\n", cod_id);
        return 0; // Blocca l'inserimento
    }
    
    if (cod_id==0){
        cod_id=((q->max_id)+1);
        q->max_id=cod_id;
        printf("Codice id assegnato:%d\n", cod_id);
    }
    if (cod_id>q->max_id){
        q->max_id=cod_id;
    }
    
    Segnalazione s = newSegnalazione(cod_id, nome, cogn, cat, desc, giorno, mese, anno, urg, stato, 0);
    if (s.cod_id == -1) return 0;
    
    aggiungi_indice(&q->indice_stato[stato], cod_id);
    aggiungi_indice(&q->indice_urgenza[urg], cod_id);
    aggiungi_indice(&q->indice_categoria[cat], cod_id);

    return insert(q, s);
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
        rimuovi_indice(&q->indice_stato[q->vet[i].stato], cod_id);
        q->vet[i].stato = nuovo_stato; 
        aggiungi_indice(&q->indice_stato[nuovo_stato], cod_id);
        printf("Nuovo stato: %d\n", q->vet[i].stato);
        return 1; 
    }

    printf("Segnalazione non trovata.\n");
    return 0; // Ritorna 0 se la segnalazione non viene trovata.
}

//Funzione per stampare un report.
void stampa_report(PQueue q){
    if (emptyPQ(q)) {
        printf("Nessuna segnalazione da visualizzare.\n");
        return;
    }
    
    int segn_cat_1 = 0, segn_cat_2 = 0, segn_cat_3 = 0, segn_cat_4 = 0, segn_cat_5 = 0;
    int seg_aperte = 0, segn_lavorazione = 0, seg_chiuse = 0;
    
    //Scorro tutta la struttura.
    for (int i = 1; i <= q->numel; i++) {
        if (q->vet[i].cat == 1) segn_cat_1++;
        else if (q->vet[i].cat == 2) segn_cat_2++;
        else if (q->vet[i].cat == 3) segn_cat_3++;
        else if (q->vet[i].cat == 4) segn_cat_4++;
        else if (q->vet[i].cat == 5) segn_cat_5++;
        
        if (q->vet[i].stato == 1) seg_aperte++;
        else if (q->vet[i].stato == 2) segn_lavorazione++;
        else if (q->vet[i].stato == 3) seg_chiuse++;
    }
    int segn_maggiori= segn_cat_1, cat_segn_maggiori =1;
    
    //Controllo per trovare la categoria con più segnalazioni.
    if (segn_cat_2 > segn_maggiori) {
        segn_maggiori = segn_cat_2;
        cat_segn_maggiori = 2;
    }
    if (segn_cat_3 > segn_maggiori) {
        segn_maggiori = segn_cat_3;
        cat_segn_maggiori = 3;
    }
    if (segn_cat_4 > segn_maggiori) {
        segn_maggiori = segn_cat_4;
        cat_segn_maggiori = 4;
    }
    if (segn_cat_5 > segn_maggiori) {
        segn_maggiori = segn_cat_5;
        cat_segn_maggiori = 5;
    }
    //Stampa del report.
    printf("\n--- REPORT ---\n");
    printf("Numero totale di segnalazioni: %d\n", q->numel);
    printf("Segnalazioni per categoria:\n"
           "Manutenzione stradale: %d\n"
           "Ambiente e rifiuti: %d\n"
           "Impianti pubblici: %d\n"
           "Segnaletica e viabilità: %d\n"
           "Altro: %d\n", segn_cat_1, segn_cat_2, segn_cat_3, segn_cat_4, segn_cat_5);
    printf("Segnalazioni aperte: %d\n", seg_aperte);
    printf("Segnalazioni in lavorazione: %d\n", segn_lavorazione);
    printf("Segnalazioni chiuse: %d\n", seg_chiuse);
    //Rende il numero della categoria più leggibile.
    const char* nome_cat_maggiore="Categoria sconosciuta";
    switch (cat_segn_maggiori) {
        case 1: nome_cat_maggiore = "Manutenzione stradale"; break;
        case 2: nome_cat_maggiore = "Ambiente e rifiuti"; break;
        case 3: nome_cat_maggiore = "Impianti pubblici"; break;
        case 4: nome_cat_maggiore = "Segnaletica e viabilità"; break;
        case 5: nome_cat_maggiore = "Altro"; break;
    }
    printf("Categoria con più segnalazioni: %s (%d segnalazioni)\n", nome_cat_maggiore, segn_maggiori);
}
