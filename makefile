affichage: affichage.o GFXLIB/libisentlib.a
	gcc -Wall affichage.o -o affichage GFXLIB/libisentlib.a -lm -lglut -lGL -lX11

affichage.o: affichage.c affichage.h GFXLIB/GfxLib.h GFXLIB/BmpLib.h GFXLIB/ESLib.h
	gcc -Wall -c affichage.c

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f affichage

