#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"
#include "affichage.h"
#include "utilisateur.h"

#define LargeurFenetre 800
#define HauteurFenetre 601

int main(int argc, char **argv) {
	initialiseGfx(argc, argv); // initialise la librairie
	
	prepareFenetreGraphique("Demineur", LargeurFenetre, HauteurFenetre); // crée une fenetre pour afficher les différents éléments

	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	
	return 0;
}
