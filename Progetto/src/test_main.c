#include <stdio.h>
#include <stdlib.h>
#include "Gestione_main.h"


int main() {
    printf("Esecuzione dei test.\n");
    FILE *f_results = fopen("results.txt", "w"); //Apre il file di testo per i risultati.
    
    test_1(f_results);
    test_2(f_results);
    test_3(f_results);
    test_4(f_results);
    test_5(f_results);
    test_6(f_results);
    test_7(f_results);
    test_8(f_results);
    fclose(f_results);

    return 0;
}