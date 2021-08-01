#Makefile para compilar el conecta 4
juego: conecta_4.o
		gcc -o juego conecta_4.o
compilar: conecta_4.c
		gcc -c conecta_4.c
clean:
		rm juego *.o