#include <stdio.h>
#include <stdlib.h>
#include "PQueue.h"

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
    printf("10. Rendi una segnalazione chiudibile\n");
    printf("11. Cambia stato di una segnalazione\n");
    printf("12. Stampa report\n");
    printf("0. Esci\n");
    printf("Scegli un'opzione: ");
    }

int main(){
    PQueue q = newPQ();
    if (!q) {
        printf("Errore nella creazione della coda di priorità.\n");
        return EXIT_FAILURE;
    }

    int scelta;

    while(1){
        menu();
        
        if (scanf("%d", &scelta) != 1) {
            printf("Opzione non valida. Per favore inserisci un numero.\n");
            while (getchar() != '\n'); // Svuota il buffer dai caratteri errati inseriti.
            continue;                  // Salta lo switch e mostra di nuovo il menu.
        }
        
        while (getchar() != '\n'); // Pulisce il buffer di input dopo la lettura della scelta.

        switch(scelta){
            case 0:
                printf("Uscita in corso...\n");
                DestroyPQueue(q);
                return 0;
            
            case 1:
                char nomefile[100];
                printf("Inserisci il nome del file da cui caricare le segnalazioni (es. segnalazioni.txt): ");
                scanf("%99s", nomefile);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                caricaDaFile(q, nomefile);
                break;
            case 2:
                printf("Inserisci i dettagli della nuova segnalazione:\n");
                printf("ID (intero positivo): ");
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
                    break;
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
                    break;
                }
                printf("Stato (1: Aperta, 2: In lavorazione, 3: Chiusa): ");
                int stato;
                scanf("%d", &stato);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (stato < 1 || stato > 3) {
                    printf("Stato non valido. Per favore inserisci un numero tra 1 e 3.\n");
                    break;
                }
                if (insertSegnalazione(q, cod_id, nome, cogn, cat, desc, giorno, mese, anno, urg, stato)) {
                printf("Segnalazione inserita con successo.\n");
                }
                else {
                    printf("Segnalazione non salvata.\n");
                }
                break;
            case 3:
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
                    return 0;
                }   
                break;
            case 4:
                visualizzaTutte(q);
                break;
            case 5:
                printf("Inserisci lo stato da cercare (1: Aperta, 2: In lavorazione, 3: Chiusa): ");
                int stato_cercato;                    
                scanf("%d", &stato_cercato);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (stato_cercato < 1 || stato_cercato > 3) {
                    printf("Stato non valido. Per favore inserisci un numero tra 1 e 3.\n");
                    break;
                }
                visualizzaPerStato(q, stato_cercato);
                break;
            case 6:
                printf("Inserisci l'urgenza da cercare (1-5): ");
                int urgenza_cercata;
                scanf("%d", &urgenza_cercata);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (urgenza_cercata < 1 || urgenza_cercata > 5) {
                    printf("Urgenza non valida. Per favore inserisci un numero tra 1 e 5.\n");
                    break;
                }
                visualizzaPerUrgenza(q, urgenza_cercata);
                break;
            case 7:
                printf("Quante segnalazioni vuoi vedere?:");
                int vedere;
                scanf("%d", &vedere);
                while (getchar() != '\n');
                visualizzaPiuUrgenti(q, vedere);
                break;
            case 8:
                printf("Inserisci il codice ID da cercare: "); 
                int codice_cercato;
                scanf("%d", &codice_cercato);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                visualizzaPerCodice(q, codice_cercato);
                break;
            case 9:
                printf("Inserisci la categoria da cercare.\n1: Manutenzione stradale\n2: Ambiente e rifiuti\n3: Impianti pubblici\n4: Segnaletica e viabilità\n5: Altro\nCategoria (1-5): ");
                int categoria_cercata;
                scanf("%d", &categoria_cercata);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (categoria_cercata < 1 || categoria_cercata > 5) {
                    printf("Categoria non valida. Per favore inserisci un numero tra 1 e 5.\n");
                    break;
                }
                visualizzaPerCategoria(q, categoria_cercata);
                break;
            case 10:
                printf("Inserisci il codice ID della segnalazione da rendere chiudibile: ");
                int cod_id_chiudibile;
                scanf("%d", &cod_id_chiudibile);
                while (getchar() != '\n'); // Pulisce il buffer di input.
                if (rendi_chiudibile(q, cod_id_chiudibile)) {
                    printf("Segnalazione resa chiudibile con successo.\n");
                }
                break;
            case 11:
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
                    break;
                }
                if (cambia_stato(q, cod_id_stato, nuovo_stato)) {
                    printf("Stato della segnalazione modificato con successo.\n");
                }
                break;
            case 12:
                stampa_report(q);
                break;
            default:
                printf("Opzione non valida. Per favore inserisci un numero tra 0 e 11.\n");
                break;
        }
    }
    return 0;
}
