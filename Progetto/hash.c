#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

//Struttura item dell'hash
struct item {
    int key;            //Chiave di ricerca, coincide con l'id delle segnalazioni
    int pos_heap;       //Posizione dell'elemento nell'heap
    struct item* next;  // Puntatore per gestire le collisioni
};

//Strtura della tabella hash
struct hash {
    int size;              //Dimensione della tabella hash
    struct item **table;   // Array di puntatori a item
};

//Funzione per creare una nuova tabella hash
hashtable newHashtable(int size) {
    hashtable h = (struct hash *) malloc(sizeof(struct hash)); //Allocazione dinamica della tabella hash
    if (h == NULL) {       
        return NULL;
    }

    h->size = size;  

    h->table = (struct item **) calloc(size, sizeof(struct item *)); //Allocazione dinamica dell'array di puntatori a item
    if (h->table == NULL) {  
        free(h);             
        return NULL;
    }

    return h;  
}

//Funzione hash per calcolare l'indice a partire dalla chiave
int hashFun(int key, int size) {
    return key % size; //La funzione è util, qualora la key diventi maggiore di size, il resto sarà il suo indice. Qualora ci siano collisioni, sono gestite nelle altre funzioni.
}

//Funzione per aggiungere un elemento alla tabella hash
//Se la chiave esiste già aggiorna solo la posizione nell'heap, altrimenti crea un nuovo nodo.
int hash_insert(hashtable h, int key, int nuova_pos_heap) {
    
    if (h == NULL || h->table == NULL) return 0;

    int indx = hashFun(key, h->size);
    struct item *head = h->table[indx];
    struct item *curr = head;

    // Cerca se la segnalazione è già registrata nella tabella hash
    while (curr != NULL) {
        if (curr->key == key) {
            // Se esiste già, aggiorna solo la sua posizione nell'heap, viene usato in sali e scendi quando la posizione nell'heap cambia
            curr->pos_heap = nuova_pos_heap;    
            return 1; 
        }
        curr = curr->next;
    }

    struct item *nuovo = (struct item *) malloc(sizeof(struct item)); //Se non esiste, crea un nuovo nodo per la tabella hash
    if (nuovo == NULL) return 0; // Errore di memoria

    nuovo->key = key;
    nuovo->pos_heap = nuova_pos_heap;
    
    // Inserimento in testa alla lista
    nuovo->next = head;
    h->table[indx] = nuovo;

    return 1; 
}

//Funzione di ricerca per trovare la posizione di una segnalazione nell'heap a partire da suo id.
int hashSearch(hashtable h, int key) {
    
    if (h == NULL || h->table == NULL) return -1;

    int indx = hashFun(key, h->size);
    struct item *curr = h->table[indx];

    while (curr != NULL) {
        if (curr->key == key) {
            return curr->pos_heap; // Restituisce la posizione nell'heap se trova la chiave.
        }
        curr = curr->next;
    }

    return -1; // Non trovato.
}

//Funzione per eliminare un elemento dalla tabella hash dato il suo id.
int hashDelete(hashtable h, int key) {
    
    if (h == NULL || h->table == NULL) return 0;
    
    int indx = hashFun(key, h->size);
    struct item *prev, *curr, *head;

    prev = curr = head = h->table[indx];

    while(curr) {
        if(curr->key == key) { // Se trova l'elemento
            if(curr == head) {
                h->table[indx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return 1;   // Eliminazione riuscita
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; // Elemento non trovato
}

// Versione iterativa di deleteList per evitare l'overflow dello stack su liste lunghe.
static void deleteList(struct item *p) {
    struct item *nextNode;
    while (p != NULL) {
        nextNode = p->next;  // Salva il riferimento al nodo successivo.
        free(p);             // Libera il nodo corrente.
        p = nextNode;        // Passa al nodo successivo.
    }
}

// Funzione per distruggere una tabella hash.
void DestroyHashtable(hashtable h) {
    
    if (h == NULL) return;

    int i;
    
    for(i = 0; i < h->size; i++) {
        // Elimina la lista collegata in ogni indice della tabella.
        deleteList(h->table[i]);
    }
    // Libera la memoria allocata per l'array di puntatori della tabella.
    free(h->table);
    // Libera la memoria allocata per la struttura della tabella hash.
    free(h);
    return;
}
