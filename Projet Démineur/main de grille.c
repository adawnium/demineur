#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"

int main() {

	bool rejouer =true;
	
    srand(time(NULL));
     while(rejouer){
     	int lignes = 0;
	int colonnes = 0;
	int mines = 0;
	int difficulte = 0;
	int x = 0;
	int y = 0;
	int choix = 0;
	int cptflag=0;
	bool caseselected= true;
	rejouer = false;
	printf("Choisissez la difficulte :\n1. Facile\n2. Moyen\n3. Difficile\n4. Test\n");
	scanf("%d", &difficulte);

	choixDifficulte(difficulte, &lignes, &colonnes, &mines, &cptflag);

	Case** tableau = initTableau(lignes, colonnes);
	afficheGrille(tableau, lignes, colonnes, cptflag);

	printf("Entrez les coordonnees de la premiere case a devoiler (y x) : ");
	scanf("%d %d", &x, &y);

	placeMines(lignes, colonnes, mines, tableau, x, y);

	for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
			compteMines(tableau, i, j, lignes, colonnes);
		}
	}

	reveleCase(tableau, x - 1, y - 1, lignes, colonnes, caseselected);

	while (!victoire(tableau, lignes, colonnes, mines)) {
		afficheGrille(tableau, lignes, colonnes, cptflag);

		printf("1. Reveler une case\n2. Placer un drapeau\n3. Enlever un drapeau\n");
		scanf("%d", &choix);
		

		switch (choix) {
		    case 1:
		    	printf("Entrez les coordonnees de la case (x y) : ");
			scanf("%d %d", &x, &y);
		        reveleCase(tableau, x - 1, y - 1, lignes, colonnes, caseselected);
		        if (tableau[x - 1][y - 1].hasMine && tableau[x - 1][y - 1].revealed) {
		            printf("PERDU TU ES BETEUH\n\n");
		            afficheGrilleComplete(tableau, lignes, colonnes);
		            return 0; // Fin du programme si perdu
		        }
		        break;
		    case 2:
		    	printf("Entrez les coordonnees de la case (x y) : ");
			scanf("%d %d", &x, &y);
		        placerDrapeau(tableau, x - 1, y - 1, &cptflag);
		        break;
		    case 3:
		    	printf("Entrez les coordonnees de la case (x y) : ");
			scanf("%d %d", &x, &y);
		        enleverDrapeau(tableau, x - 1, y - 1, &cptflag);
		        break;
		    default:
		        printf("Veuillez choisir un chiffre entre 1 et 3\n");
		        break;
		}

	}

	if (victoire(tableau, lignes, colonnes, mines)) {
		printf("WHOWHWOHWOWHOHWOWHOHWOWHOWHOWHOHWOWHHO SPECE DE NOOOOOB T'A GAGNE\n");
		afficheGrilleComplete(tableau, lignes, colonnes);
	}
	printf("Veux tu rejouer ? (oui = 1)\n");
	scanf("%d", &choix);

	if (choix == 1) {
		rejouer= true;
	}
	
	libererGrille(tableau, lignes);
	}

	return 0;
}
