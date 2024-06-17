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

#define LargeurFenetre 800  // Augmenté pour inclure l'espace du score
#define HauteurFenetre 600
#define LARGEUR_SCORE 400

void gererClicGauche(int x, int y, int *lignes, int *colonnes, int *difficulte, int *mines, int *cptflag, bool *dehors, bool *firstclick, Case **tableau, bool *minetouche, bool *win, bool *initialise, time_t *startTime)
{	
	float lFenetre = largeurFenetre();
	float hFenetre = hauteurFenetre();
		
	if (*difficulte != 0 && !*win && !*minetouche)
	{
		int largeur = largeurFenetre() - LARGEUR_SCORE; // Réduire la largeur pour inclure l'espace du score
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
			if (*firstclick == false)
			{	
				placeMines(*lignes, *colonnes, *mines, tableau, ligneCliquee, colonneCliquee);
				for (int i = 0; i < *lignes; i++)
				{
					for (int j = 0; j < *colonnes; j++)
					{
						compteMines(tableau, i, j, *lignes, *colonnes);
					}
				}
				bool caseselected = true;
				reveleCase(tableau, ligneCliquee, colonneCliquee, *lignes, *colonnes, caseselected);
				*startTime = time(NULL);
				*firstclick = true;
			} else
			{
				if (!tableau[ligneCliquee][colonneCliquee].hasMine)
				{
					bool caseselected = true;
					reveleCase(tableau, ligneCliquee, colonneCliquee, *lignes, *colonnes, caseselected);
					if(victoire(tableau, *lignes, *colonnes, *mines)){
						*win = true;
					}
				}
				else
				{
					*minetouche = true;
				}
			}
		}

		else
		{
			*dehors = true;
		}
		float rectangles[3][4] = {
		{lFenetre * 5 / 8, hFenetre * 7 / 12, lFenetre * 7 / 8, hFenetre * 5.5 / 12},
		{lFenetre * 5 / 8, hFenetre * 4.5 / 12, lFenetre * 7 / 8, hFenetre * 3 / 12},
		{lFenetre * 5 / 8, hFenetre * 2 / 12, lFenetre * 7 / 8, hFenetre * 0.5 / 12}};

	    for (int i = 0; i < 3; i++) {
		if (x >= rectangles[i][0] && x <= rectangles[i][2] && y >= rectangles[i][3] && y <= rectangles[i][1]){
		    switch (i) {
		        case 0:
		            // Recommencer
		            	libererGrille(tableau, *lignes);
		            	*initialise = false;
    				*firstclick = false;
    				*cptflag= *mines;
    				printf("Vous avez recommencé\n");
		            break;
		        case 1:
		            // niveau suivant
		            
		            switch(*difficulte){
		            	case 1:
		            		*difficulte = 2;
					*lignes = 0;
					*colonnes = 0;
					*mines = 0;
					*cptflag = 0;
					libererGrille(tableau, *lignes);
					choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
					*initialise = false;
					*firstclick = false;
					redimensionneFenetre(857, HauteurFenetre);
		            		break;
		            	case 2:
		            		*difficulte = 3;
					*lignes = 0;
					*colonnes = 0;
					*mines = 0;
					*cptflag = 0;
					libererGrille(tableau, *lignes);
					choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
					*initialise = false;
					*firstclick = false;
					redimensionneFenetre(882, HauteurFenetre);
		            		break;
		            	case 3:
		            		*difficulte = 1;
					*lignes = 0;
					*colonnes = 0;
					*mines = 0;
					*cptflag = 0;
					libererGrille(tableau, *lignes);
					choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
					*initialise = false;
					*firstclick = false;
					redimensionneFenetre(LargeurFenetre, HauteurFenetre);
		            		break;
		            }
		            	
		            break;
		        case 2:
		            // écran titre
		            *difficulte = 0;
			    *lignes = 0;
			    *colonnes = 0;
			    *mines = 0;
			    *cptflag = 0;
			    tableau = NULL;
			    *initialise = false;
			    *firstclick = false;
		            break;
		    }
		}
	    }
	    
	}

	if (*difficulte == 0)
	{

		// Coordonnées des rectangles
		float facileX1 = lFenetre * 3 / 8, facileY1 = hFenetre * 9.5 / 12, facileX2 = lFenetre * 5 / 8, facileY2 = hFenetre * 7.5 / 12;
		float moyenX1 = lFenetre * 3 / 8, moyenY1 = hFenetre * 6.5 / 12, moyenX2 = lFenetre * 5 / 8, moyenY2 = hFenetre * 4.5 / 12;
		float difficileX1 = lFenetre * 3 / 8, difficileY1 = hFenetre * 3.5 / 12, difficileX2 = lFenetre * 5 / 8, difficileY2 = hFenetre * 1.5 / 12;

		// Vérifier si le clic est sur le bouton "FACILE"
		if (x >= facileX1 && x <= facileX2 && y >= facileY2 && y <= facileY1)
		{

			*difficulte = 1;
		}
		// Vérifier si le clic est sur le bouton "MOYEN"
		else if (x >= moyenX1 && x <= moyenX2 && y >= moyenY2 && y <= moyenY1)
		{
			*difficulte = 2;
			redimensionneFenetre(857, HauteurFenetre);
		}
		// Vérifier si le clic est sur le bouton "DIFFICILE"
		else if (x >= difficileX1 && x <= difficileX2 && y >= difficileY2 && y <= difficileY1)
		{
			*difficulte = 3;
			redimensionneFenetre(882, HauteurFenetre);
		}
		choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
	}
	
}


void gererClicDroit(int x, int y, int lignes, int colonnes, int difficulte, Case** tableau, int *cptflag)
{
    if(difficulte != 0){
	    int largeur = largeurFenetre() - LARGEUR_SCORE;  // Réduire la largeur pour inclure l'espace du score
	    int hauteur = hauteurFenetre();
	    int largeurCase = largeur / colonnes;
	    int hauteurCase = hauteur / lignes;
	    int tailleCase = largeurCase < hauteurCase ? largeurCase : hauteurCase;

	    // Convertir les coordonnées du clic en coordonnées de la grille
	    int colonneCliquee = x / tailleCase;
	    int ligneCliquee = lignes - 1 - (y / tailleCase);

	    // Vérifier si le clic est dans les limites de la grille
	    if (colonneCliquee >= 0 && colonneCliquee < colonnes && ligneCliquee >= 0 && ligneCliquee < lignes)
	    {
		printf("Case cliquée: (%d, %d)\n", ligneCliquee, colonneCliquee);
		if(tableau[ligneCliquee][colonneCliquee].flagged == false){
			placerDrapeau(tableau, ligneCliquee, colonneCliquee, cptflag);
		}
		else{
			enleverDrapeau(tableau, ligneCliquee, colonneCliquee, cptflag);
		}
	    }
    }
}
