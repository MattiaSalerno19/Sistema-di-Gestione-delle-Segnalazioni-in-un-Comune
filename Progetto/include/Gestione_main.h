#include <stdio.h>
#include <stdlib.h>
#include "PQueue.h"

//Prototipi delle funzioni
void menu();
void inserisci_file_main(PQueue q);
void inserisci_main(PQueue q);
int elimina_main(PQueue q);
void cerca_stato_main(PQueue q);
void cerca_urgenza_main(PQueue q);
void vedi_prime_main(PQueue q);
void cerca_id_main(PQueue q);
void cerca_categoria_main(PQueue q);
void rendi_rimovibile_main(PQueue q);
int cambia_stato_main(PQueue q);
void test_1(FILE *f_results);
void test_2(FILE *f_results);
void test_3(FILE *f_results);
void test_4(FILE *f_results);
void test_5(FILE *f_results);
void test_6(FILE *f_results);
void test_7(FILE *f_results);
void test_8(FILE *f_results);