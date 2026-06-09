#include <stdio.h>
#include <stdlib.h>
#include "Gestione_main.h"

//Menu di scelta.
void menu() {
    printf("\n--- MENU ---\n");
    printf("1. Inserisci segnalazioni tramite file di testo\n");
    printf("2. Inserisci una nuova segnalazione\n");
    printf("3. Elimina una segnalazione\n");
    printf("4. Visualizza tutte le segnalazioni\n");
    printf("5. Visualizza segnalazioni per stato\n");
    printf("6. Visualizza segnalazioni per urgenza\n");
    printf("7. Visualizza le segnalazioni più urgenti\n");
    printf("8. Visualizza segnalazioni per codice id\n");
    printf("9. Visualizza segnalazioni per categoria\n");
    printf("10. Rendi una segnalazione rimovibile\n");
    printf("11. Cambia stato di una segnalazione\n");
    printf("12. Stampa report\n");
    printf("0. Esci\n");
    printf("Scegli un'opzione: ");
}

//Inserimento tramite nome di file.
void inserisci_file_main(PQueue q){
    char nomefile[100];
    printf("Inserisci il nome del file da cui caricare le segnalazioni (es. segnalazioni.txt, il file va posizionato interno alla cartella del progetto): ");
    scanf("%99s", nomefile);
    while (getchar() != '\n'); // Pulisce il buffer di input.
    caricaDaFile(q, nomefile);
}

//Inserimento manuale singolo.
void inserisci_main(PQueue q){
    printf("Inserisci i dettagli della nuova segnalazione:\n");
                //Prende in input i vari dati da tastiera.
                printf("ID (intero positivo, inserire 0 per generare un codice ID automaticamente): ");
                int cod_id;
                scanf("%d", &cod_id);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                printf("Nome del cittadino: ");
                char nome[50];
                scanf("%49s", nome);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                printf("Cognome del cittadino: ");
                char cogn[50];
                scanf("%49s", cogn);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                printf("Inserisci la categoria da cercare.\n1: Manutenzione stradale\n2: Ambiente e rifiuti\n3: Impianti pubblici\n4: Segnaletica e viabilità\n5: Altro\nCategoria (1-5): ");
                int cat;
                scanf("%d", &cat);
                while (getchar() != '\n'); // Pulisce il buffer di input.   
                if (cat < 1 || cat > 5) {
                    printf("Categoria non valida. Per favore inserisci un numero tra 1 e 5.\n");
                    return;
                }
                printf("Descrizione: ");
                char desc[200];
                scanf("%199[^\n]", desc);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                printf("Data (giorno mese anno): ");
                int giorno, mese, anno;
                scanf("%d %d %d", &giorno, &mese, &anno);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                printf("Urgenza (1-5): ");
                int urg;
                scanf("%d", &urg);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (urg < 1 || urg > 5) {
                    printf("Urgenza non valida. Per favore inserisci un numero tra 1 e 5.\n"); 
                    return;
                }
                printf("Stato (1: Aperta, 2: In lavorazione, 3: Chiusa): ");
                int stato;
                scanf("%d", &stato);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (stato < 1 || stato > 3) {
                    printf("Stato non valido. Per favore inserisci un numero tra 1 e 3.\n");
                    return;
                }
                //Prova a inserire la segnalazione.
                if (insertSegnalazione(q, cod_id, nome, cogn, cat, desc, giorno, mese, anno, urg, stato)) {
                printf("Segnalazione inserita con successo.\n");
                }
                else {
                    printf("Segnalazione non salvata.\n");
                }
}

//Eliminazione di una segnalazione.
int elimina_main(PQueue q){
    printf("Inserisci il codice ID della segnalazione da eliminare: ");
                int cod_id_elimina;
                scanf("%d", &cod_id_elimina);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                int rimossa = delete(q, cod_id_elimina);
                if (rimossa==1) {
                    printf("Segnalazione eliminata con successo.\n");
                }
                else if (rimossa==2) {
                    DestroyPQueue(q);
                    return 1; //Ritorna 1 in caso di errore irrevertibile.
                }
                return 0;
}

//Ricerca per stato.
void cerca_stato_main (PQueue q){
    printf("Inserisci lo stato da cercare (1: Aperta, 2: In lavorazione, 3: Chiusa): ");
                int stato_cercato;                    
                scanf("%d", &stato_cercato);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (stato_cercato < 1 || stato_cercato > 3) {
                    printf("Stato non valido. Per favore inserisci un numero tra 1 e 3.\n");
                    return;
                }
                visualizzaPerStato(q, stato_cercato);
}

//Ricerca per urgenza.
void cerca_urgenza_main (PQueue q){
    printf("Inserisci l'urgenza da cercare (1-5): ");
                int urgenza_cercata;
                scanf("%d", &urgenza_cercata);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (urgenza_cercata < 1 || urgenza_cercata > 5) {
                    printf("Urgenza non valida. Per favore inserisci un numero tra 1 e 5.\n");
                    return;
                }
                visualizzaPerUrgenza(q, urgenza_cercata);
}

//Visualizzazione prime segnalazioni.
void vedi_prime_main (PQueue q){
    printf("Quante segnalazioni vuoi vedere?:");
                int vedere;
                scanf("%d", &vedere);
                while (getchar() != '\n');
                visualizzaPiuUrgenti(q, vedere);
}

//Ricerca per id.
void cerca_id_main (PQueue q){
    printf("Inserisci il codice ID da cercare: "); 
                    int codice_cercato;
                    scanf("%d", &codice_cercato);
                    while (getchar() != '\n'); // Pulisce il buffer di input.
                    visualizzaPerCodice(q, codice_cercato);
}

//Ricerca per categoria.
void cerca_categoria_main(PQueue q){
    printf("Inserisci la categoria da cercare.\n1: Manutenzione stradale\n2: Ambiente e rifiuti\n3: Impianti pubblici\n4: Segnaletica e viabilità\n5: Altro\nCategoria (1-5): ");
                int categoria_cercata;
                scanf("%d", &categoria_cercata);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (categoria_cercata < 1 || categoria_cercata > 5) {
                    printf("Categoria non valida. Per favore inserisci un numero tra 1 e 5.\n");
                    return;
                }
                visualizzaPerCategoria(q, categoria_cercata);
}

//Cambio rimovibilità di una segnalazione.
void rendi_rimovibile_main(PQueue q){
    printf("Inserisci il codice ID della segnalazione da rendere rimovibile: ");
                    int cod_id_rimozione;
                    scanf("%d", &cod_id_rimozione);
                    while (getchar() != '\n'); // Pulisce il buffer di input.
                    if (rendi_rimovibile(q, cod_id_rimozione)) {
                        printf("Segnalazione resa rimovibile con successo.\n");
                    }
}

//Cambio stato di una segnalazione.
int cambia_stato_main(PQueue q){
    printf("Inserisci il codice ID della segnalazione da modificare: ");
    int cod_id_stato;
    scanf("%d", &cod_id_stato);
    while (getchar() != '\n'); // Pulisce il buffer di input.
    printf("Inserisci il nuovo stato (1: Aperta, 2: In lavorazione, 3: Chiusa): ");
    int nuovo_stato;
    scanf("%d", &nuovo_stato);
    while (getchar() != '\n'); // Pulisce il buffer di input.
    if (nuovo_stato < 1 || nuovo_stato > 3) {
        printf("Stato non valido. Per favore inserisci un numero tra 1 e 3.\n");
        return 0;
    }
    if (cambia_stato(q, cod_id_stato, nuovo_stato)) {
        printf("Stato della segnalazione modificato con successo.\n");
        if (nuovo_stato==3){ //Qualora la segnalazione sia resa chiusa, si chiede se si vuole subito rimuoverla.
            printf("La segnalazione è ora chiusa: vuoi già rimuoverla dal sistema? (La rimozione è permanente):\n");
            int rimozione;
            while(1){
                printf("Seleziona 0 per lasciarla, 1 per rimuoverla:");
                scanf("%d", &rimozione);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (rimozione==0){
                    printf("Segnalazione non rimossa.\n");
                    break;
                }
                else if(rimozione==1){
                    rendi_rimovibile(q, cod_id_stato);
                    int rimossa = (delete(q, cod_id_stato));
                    if (rimossa==1) {
                        printf("Segnalazione eliminata con successo.\n");
                        break;
                    }
                    else if (rimossa==2) {
                        DestroyPQueue(q);
                        return 1; //Ritorna 1 in caso di errore irrevertibile.
                    }          
            
                }   
                printf("Selezione non valida.\n");
            }
            return 0;
        }
    } 
    return 0;
}

void test_1(FILE *f_results){ //Test riguardante l'inserimento di dati errati
    
    FILE *tcase = fopen("test/TC_1.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_1.txt", "r");
    FILE *output = freopen("test/output_TC_1.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 1 (dati errati) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 1 (dati errati) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_1.txt");
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
            fprintf(f_results, "Test 1 (dati errati) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 1 (dati errati) superato.\n");
    return; //Test superato

}

void test_2(FILE *f_results){ //Tets riguardante la ricerca delle segnalazioni
    

    FILE *tcase = fopen("test/TC_2.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_2.txt", "r");
    FILE *output = freopen("test/output_TC_2.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 2 (ricerca segnalazione) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 2 (ricerca segnalazione) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_2.txt");
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
            fprintf(f_results, "Test 2 (ricerca segnalazione) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 2 (ricerca segnalazione) superato.\n");
    return; //Test superato

}

void test_3(FILE *f_results){ //Test riguardante il cambiamento di stato

    FILE *tcase = fopen("test/TC_3.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_3.txt", "r");
    FILE *output = freopen("test/output_TC_3.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 3 (cambiamento di stato) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 3 (cambiamento di stato) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_3.txt");
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
            fprintf(f_results, "Test 3 (cambiamento di stato) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 3 (cambiamento di stato) superato.\n");
    return; //Test superato

}

void test_4(FILE *f_results){ //Test riguardante la gestione di priorità

    FILE *tcase = fopen("test/TC_4.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_4.txt", "r");
    FILE *output = freopen("test/output_TC_3.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 4 (gestione priorità) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 4 (gestione priorità) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_4.txt");
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
            fprintf(f_results, "Test 4 (gestione priorità) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 4 (gestione priorità) superato.\n");
    return; //Test superato

}

void test_5(FILE *f_results){ //Test riguardante la ricerca filtrata

    FILE *tcase = fopen("test/TC_5.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_5.txt", "r");
    FILE *output = freopen("test/output_TC_5.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 5 (ricerca filtrata) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 5 (ricerca filtrata) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_5.txt");
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
            fprintf(f_results, "Test 5 (ricerca filtrata) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 5 (ricerca filtrata) superato.\n");
    return; //Test superato

}

void test_6(FILE *f_results){ //Test riguardante la generazione del report
    
    FILE *tcase = fopen("test/TC_6.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_6.txt", "r");
    FILE *output = freopen("test/output_TC_6.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 6 (generazione report) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 6 (generazione report) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_6.txt");
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
            fprintf(f_results, "Test 6 (generazione report) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 6 (generazione report) superato.\n");
    return; //Test superato

}

void test_7(FILE *f_results){ //Test riguradante l'eliminazione di una segnalazione

    FILE *tcase = fopen("test/TC_7.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_7.txt", "r");
    FILE *output = freopen("test/output_TC_7.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 7 (eliminazione segnalazione) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 7 (eliminazione segnalazione) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_7.txt");
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
            fprintf(f_results, "Test 7 (eliminazione segnalazione) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 7 (eliminazione segnalazione) superato.\n");
    return; //Test superato

}

void test_8(FILE *f_results){ //Test riguardante la visione delle segnalazioni più urgenti

    FILE *tcase = fopen("test/TC_8.txt", "r");
    FILE *oracle = fopen("test/Oracolo_TC_8.txt", "r");
    FILE *output = freopen("test/output_TC_1.txt", "w+", stdout);

    if (!output || !oracle || !tcase) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nell'apertura dei file. Test 8 (visualizzazione più urgenti) fallito.\n");
        return;
    }

    PQueue q = newPQ();
    if (!q) {
        fclose(tcase);
        fclose(oracle);     
        fclose(output);
        freopen("CON", "w", stdout); // Ripristina l'output su console
        fprintf(f_results, "Errore nella creazione della coda di priorità. Test 8 (visualizzazione più urgenti) fallito.\n");
        return;
    }

    caricaDaFile(q, "test/TC_8.txt");
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
            fprintf(f_results, "Test 8 (visualizzazione più urgenti) fallito.\n");
            return; //Test fallito 
        }

        if (c1 == EOF && c2 == EOF) {
            break; // Fine dei file, test superato
        }
    }
            
    fclose(tcase);
    fclose(oracle);
    fclose(output);
    freopen("CON", "w", stdout); // Ripristina l'output su console
    fprintf(f_results, "Test 8 (visualizzazione più urgenti) superato.\n");
    return; //Test superato

}
