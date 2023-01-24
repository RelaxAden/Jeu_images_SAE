main: fonctions.o main.o
	gcc -o main fonctions.o main.o -lSDL2 -lSDL2_image -lSDL2_ttf

fonctions.o: fonctions.c fonctions.h
	gcc -o fonctions.o -c fonctions.c

main.o: main.c fonctions.h
	gcc -o main.o -c main.c 