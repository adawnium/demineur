exec: main.o grille.o affichage.o utilisateur.o GFXLIB/libisentlib.a
	gcc -Wall -o exec main.o grille.o affichage.o utilisateur.o GFXLIB/libisentlib.a -lm -lglut -lGL -lX11

	
main.o: main.c GFXLIB/GfxLib.h
	gcc -Wall -c main.c

grille.o: grille.c grille.h GFXLIB/GfxLib.h
	gcc -Wall -c grille.c
	
affichage.o: affichage.c affichage.h GFXLIB/GfxLib.h
	gcc -Wall -c affichage.c
	
utilisateur.o: utilisateur.c utilisateur.h GFXLIB/GfxLib.h
	gcc -Wall -c utilisateur.c

clean:
	rm -f *~ *.o
