all: link test

link: hash.o Pqueue.o main.o
	gcc hash.o Pqueue.o main.o -o main.exe

test: hash.o Pqueue.o test_main.o
	gcc hash.o Pqueue.o test_main.o -o test.exe
    
hash.o:
	gcc -c hash.c

Pqueue.o:
	gcc -c Pqueue.c

main.o:
	gcc -c main.c

test_main.o:
	gcc -c test_main.c

clean:
	del /f /q *.o main.exe test.exe output_test_*.txt