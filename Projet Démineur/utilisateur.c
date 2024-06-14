#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <stdbool.h>
#include <time.h>
#include "grille.h"
#include "GFXLIB/GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "GFXLIB/BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "GFXLIB/ESLib.h" // Pour utiliser valeurAleatoire()
#include "affichage.h"
#include "utilisateur.h"

#define LargeurFenetre 900 
#define HauteurFenetre 600
#define LARGEUR_SCORE 350

void gererClicGauche(int x, int y, int *lignes, int *colonnes, int *difficulte, int *mines, int *cptflag)
{
    if(*difficulte != 0){
	    int largeur = largeurFenetre() - LARGEUR_SCORE;  // Réduire la largeur pour inclure l'espace du score
	    int hauteur = hauteurFenetre();
	    int largeurCase = largeur / *colonnes;
	    int hauteurCase = hauteur / *lignes;
	    int tailleCase = largeurCase < hauteurCase ? largeurCase : hauteurCase;

	    // Convertir les coordonnées du clic en coordonnées de la grille
	    int colonneCliquee = x / tailleCase;
	    int ligneCliquee = *lignes - 1 - (y / tailleCase);

	    // Vérifier si le clic est dans les limites de la grille
	    if (colonneCliquee >= 0 && colonneCliquee < *colonnes && ligneCliquee >= 0 && ligneCliquee < *lignes)
	    {
		printf("Case cliquée: (%d, %d)\n", ligneCliquee, colonneCliquee);
		// Ajouter le code pour révéler la case ou placer/enlever un drapeau ici
		// Exemple:
		// reveleCase(tableau, ligneCliquee, colonneCliquee, LIGNES, COLONNES, caseselected);
	    }
    }
    if(*difficulte == 0){
    	float lFenetre = largeurFenetre();
	float hFenetre = hauteurFenetre();

	// Coordonnées des rectangles
	float facileX1 = lFenetre*3/8, facileY1 = hFenetre*9.5/12, facileX2 = lFenetre*5/8, facileY2 = hFenetre*7.5/12;
	float moyenX1 = lFenetre*3/8, moyenY1 = hFenetre*6.5/12, moyenX2 = lFenetre*5/8, moyenY2 = hFenetre*4.5/12;
	float difficileX1 = lFenetre*3/8, difficileY1 = hFenetre*3.5/12, difficileX2 = lFenetre*5/8, difficileY2 = hFenetre*1.5/12;

	// Vérifier si le clic est sur le bouton "FACILE"
	if (x >= facileX1 && x <= facileX2 && y >= facileY2 && y <= facileY1)
	    {
	   
		*difficulte = 1;
	    }
	    // Vérifier si le clic est sur le bouton "MOYEN"
	    else if (x >= moyenX1 && x <= moyenX2 && y >= moyenY2 && y <= moyenY1)
	    {
		*difficulte = 2;
	    }
	    // Vérifier si le clic est sur le bouton "DIFFICILE"
	    else if (x >= difficileX1 && x <= difficileX2 && y >= difficileY2 && y <= difficileY1)
	    {
		*difficulte = 3;
	}
	choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
}
}



