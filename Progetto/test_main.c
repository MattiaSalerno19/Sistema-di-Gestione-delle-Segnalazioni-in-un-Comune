#include <stdio.h>
#include <stdlib.h>
#include "PQueue.h"

int test_1(){ //Test riguardante l'inserimento di dati errati
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_1.txt");
    sprintf(oracle_fname, "Oracolo_TC_1.txt");
    sprintf(output_fname, "output_test_1.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    visualizzaTutte(q); 

    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_2(){ //Tets riguardante la ricerca delle segnalazioni
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_2.txt");
    sprintf(oracle_fname, "Oracolo_TC_2.txt");
    sprintf(output_fname, "output_test_2.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    visualizzaPerCodice(q, 10); //Ricerco una segnalazione esistente
    visualizzaPerCodice(q, 13); //Ricerco una segnalazione non esistente
    visualizzaPerCodice(q, 18); //Ripeto la ricerca di una segnalazione esistente
    visualizzaPerCodice(q, 20); //Ripeto la ricerca di una segnalazione non eistente

    rewind(output); // Torna all'inizio del file di output


    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_3(){ //Test riguardante il cambiamento di stato

    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_3.txt");
    sprintf(oracle_fname, "Oracolo_TC_3.txt");
    sprintf(output_fname, "output_test_3.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    cambia_stato(q, 2, 2); //Cambio a 2
    cambia_stato(q, 5, 3); //Cambio a 3
    cambia_stato(q, 13, 1); //Cambio a 1
    cambia_stato(q, 18, 1); //Cambio allo stesso stato

    rewind(output); // Torna all'inizio del file di output


    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_4(){ //Test riguardante la gestione di priorità
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_4.txt");
    sprintf(oracle_fname, "Oracolo_TC_4.txt");
    sprintf(output_fname, "output_test_4.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    visualizzaTutte(q);

    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_5(){ //Test riguardante la ricerca filtrata
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_5.txt");
    sprintf(oracle_fname, "Oracolo_TC_5.txt");
    sprintf(output_fname, "output_test_5.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    visualizzaPerStato(q, 1); //Stato 1
    visualizzaPerStato(q, 2); //Stato 2
    visualizzaPerStato(q, 3); //Stato 3
    visualizzaPerUrgenza(q, 1); //Urgenza 1
    visualizzaPerUrgenza(q, 2); //Urgenza 2
    visualizzaPerUrgenza(q, 3); //Urgenza 3
    visualizzaPerUrgenza(q, 4); //Urgenza 4
    visualizzaPerUrgenza(q, 5); //Urgenza 5
    visualizzaPerCategoria(q, 1); //Categoria 1
    visualizzaPerCategoria(q, 2); //Categoria 2
    visualizzaPerCategoria(q, 3); //Categoria 3
    visualizzaPerCategoria(q, 4); //Categoria 4
    visualizzaPerCategoria(q, 5); //Categoria 5

    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_6(){ //Test riguardante la generazione del report
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_6.txt");
    sprintf(oracle_fname, "Oracolo_TC_6.txt");
    sprintf(output_fname, "output_test_6.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    stampa_report(q);
     
    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_7(){ //Test riguradante l'eliminazione di una segnalazione
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_7.txt");
    sprintf(oracle_fname, "Oracolo_TC_7.txt");
    sprintf(output_fname, "output_test_7.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    delete(q, 2); //Provo ad eliminare una segnalazione non chiudibile
    visualizzaPerCodice(q, 2); //Controllo che la segnalazione non sia stata eliminata
    rendi_rimovibile(q, 2); //Rendo chiudibile la segnalazione
    delete(q, 2); //Provo ad eliminarla dopo il cambiamento
    visualizzaPerCodice(q, 2); //Controllo che sia sta eliminata

    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int test_8(){ //Test riguardante la visione delle segnalazioni più urgenti
    
    char input_fname[100];
    char output_fname[100];
    char oracle_fname[100];

    sprintf(input_fname, "TC_8.txt");
    sprintf(oracle_fname, "Oracolo_TC_8.txt");
    sprintf(output_fname, "output_test_8.txt");

    FILE *tcase = fopen(input_fname, "r");
    FILE *oracle = fopen(oracle_fname, "r");
    FILE *output = freopen(output_fname, "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nell'apertura dei file .\n");
        return 0;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        printf("Errore nella creazione della coda di priorità.\n");
        return 0;
    }

    caricaDaFile(q, input_fname);
    visualizzaPiuUrgenti(q, 20); //Più di quante ce ne siano
    visualizzaPiuUrgenti(q, -2); //Numero minore di 0
    visualizzaPiuUrgenti(q, 4);

    rewind(output); // Torna all'inizio del file di output
    
    int c1, c2;
    while (1){ //Confronto i file di output e oracolo
        c1 = fgetc(oracle);
        c2 = fgetc(output);

        if (c1 != c2) {
            fclose(tcase);
            fclose(oracle);
            fclose(output);
            freopen("CON", "w", stdout); // Ripristina l'output su console
            return 0; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    return 1; //Test superato

}

int main() {
    printf("Esecuzione dei test.\n");
    if (test_1()) {
        printf("Test 1 (dati errati) superato.\n");
    } 
    else {
        printf("Test 1 (dati errati) fallito.\n");
    }
    if (test_2()) {
        printf("Test 2 (ricerca segnalazione) superato.\n");
    } 
    else {
        printf("Test 2 (ricerca segnalazione) fallito.\n");
    }
    if (test_3()) {
        printf("Test 3 (cambiamento di stato) superato.\n");
    } 
    else {
        printf("Test 3 (cambiamento di stato) fallito.\n");
    }
    if (test_4()) {
        printf("Test 4 (gestione priorità) superato.\n");
    } 
    else {
        printf("Test 4 (gestione priorità) fallito.\n");
    }
    if (test_5()) {
        printf("Test 5 (ricerca filtrata) superato.\n");
    } 
    else {
        printf("Test 5 (ricerca filtrata) fallito.\n");
    }
    if (test_6()) {
        printf("Test 6 (generazione report) superato.\n");
    } 
    else {
        printf("Test 6 (generazione report) fallito.\n");
    }
    if (test_7()) {
        printf("Test 7 (eliminazione segnalazione) superato.\n");
    } 
    else {
        printf("Test 7 (eliminazione segnalazione) fallito.\n");
    }
    if (test_8()){
        printf("Test 8 (visualizzazione più urgenti) superato.\n");
    }
    else{
        printf("Test 8 (visualizzazione più urgenti) fallito.\n");
    }

    return 0;
}