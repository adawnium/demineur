#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"
#include "affichage.h"
#include "utilisateur.h"

#define LargeurFenetre 900
#define HauteurFenetre 600

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("beteuh", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}
