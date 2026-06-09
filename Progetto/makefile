#Definizione del compilatore.
CC = gcc
#Warning attivi, prende gli header dalla cartella include.
CFLAGS = -Iinclude -Wall -g
#Cartella per i .o
OBJ_DIR = obj
#Cartella per i .c
SRC_DIR = src

#File in comune tra i due main
SRCS_COMMON = $(SRC_DIR)/hash.c $(SRC_DIR)/PQueue.c $(SRC_DIR)/Gestione_main.c

SRC_MAIN = $(SRC_DIR)/main.c 
SRC_TEST = $(SRC_DIR)/test_main.c

#La lista di .c diventa di .o
OBJS_COMMON = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_COMMON)) 
OBJ_MAIN = $(OBJ_DIR)/main.o
OBJ_TEST = $(OBJ_DIR)/test_main.o

#Nomi per gli eseguibili.
TARGET_MAIN = main.exe
TARGET_TEST = test.exe

#Compila tutto.
all: $(TARGET_MAIN) $(TARGET_TEST)

#Link dei file per il main principiale.
$(TARGET_MAIN): $(OBJS_COMMON) $(OBJ_MAIN)
	$(CC) $(OBJS_COMMON) $(OBJ_MAIN) -o $(TARGET_MAIN)

#Link dei file per il main test.
$(TARGET_TEST): $(OBJS_COMMON) $(OBJ_TEST)
	$(CC) $(OBJS_COMMON) $(OBJ_TEST) -o $(TARGET_TEST)

#Rende i .c dei .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Se non esiste, crea la cartella obj.
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

#Pulizia per rimuovere i .o, i .exe, gli outuput e il file dei risultati.
clean:
	del /f /q $(OBJ_DIR)\*.o *.exe test\output_test_*.txt results.txt