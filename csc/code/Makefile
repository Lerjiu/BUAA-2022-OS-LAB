fibo: main.o fibo.o
	gcc main.o fibo.o -o ../fibo
main.o: main.c 
	gcc -c main.c -o main.o -I ../include
fibo.o: fibo.c
	gcc -c fibo.c -o fibo.o -I ../include
clean:
	rm main.o fibo.o
