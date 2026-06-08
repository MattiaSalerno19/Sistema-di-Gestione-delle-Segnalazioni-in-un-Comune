#include <stdio.h>
#include <stdlib.h>
#include "Gestione.h"


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
                inserisci_file_main(q);
                break;
            case 2:
                inserisci_main(q);
                break;
            case 3:
                if(elimina_main(q)) return 0;   
                break;
            case 4:
                visualizzaTutte(q);
                break;
            case 5:
                cerca_stato_main(q);
                break;
            case 6:
                cerca_urgenza_main(q);
                break;
            case 7:
                vedi_prime_main(q);
                break;
            case 8:
                cerca_id_main(q);
                break;
            case 9:
                cerca_categoria_main(q);
                break;
            case 10:
                rendi_rimovibile_main(q);
                break;
            case 11:
                if (cambia_stato_main(q)) return 0;
                break;           
            case 12:
                stampa_report(q);
                break;
            default:
                printf("Opzione non valida. Per favore inserisci un numero tra 0 e 12.\n");
                break;
        }
    }
    return 0;
}
