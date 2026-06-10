# Sistema di gestione delle segnalazioni in un Comune

* **Sistema Operativo:** Windows
* **Linguaggio:** C99

## Struttura 
La repository è organizzata come segue:
* `src/` : Contiene i file sorgente del codice (`.c`)
* `include/` : Contiene i file di intestazione (`.h`)
* `data/` : Contiene i file di testo per l'input/output dei dati

Queste 3 cartelle sono presenti all'interno della cartella "Progetto"
* `Makefile` : File di configurazione per l'automazione della compilazione
* `README.md` : Questo file con le istruzioni

[!WARNING]
> **CARTELLE OBSOLETE - NON UTILIZZARE**
> Le cartelle `Progetto PSD` e `Progeto_PSD`, così come tutti i file all'esterno della cartella `Progetto` (esclusi il file `Report.pdf` e questo `README.md`), contengono versioni vecchie o errate e sono mantenute esclusivamente come storico.

---

## Istruzioni per la Compilazione ed Esecuzione

*Posizionarsi nella repository corretta:*
```bash
cd Progetto
```
*Compilazione:*
```bash
mingw32-make
```
*Esecuzione del programma:*
```bash
./main.exe
```
*Esecuzione dei test:*
```bash
./test.exe
```
*Pulizia:*
```bash
mingw32-make clean
```

