link: hash.o Pqueue.o main.o
	gcc hash.o Pqueue.o main.o -o main.exe

hash.o:
	gcc -c hash.c

Pqueue.o:
	gcc -c Pqueue.c

main.o:
	gcc -c main.c

clean:
	del /f /q *.o main.exe

