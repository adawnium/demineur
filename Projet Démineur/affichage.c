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

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 900  // Augmenté pour inclure l'espace du score
#define HauteurFenetre 600
#define LARGEUR_SCORE 350

void gestionEvenement(EvenementGfx evenement) {
	static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou pas
	static bool fondBlanc = true;
	static int difficulte = 0;
	static int lignes = 0;
	static int colonnes = 0;
	static int mines = 0;
	static int cptflag = 0;
	Case** tableau = NULL;
	

	switch (evenement)
	{
		case Initialisation:

			demandeTemporisation(20);
			break;
		
		case Temporisation:
			rafraichisFenetre();
			break;
			
		case Affichage:
			if(fondBlanc == true) {
				effaceFenetre (255, 255, 255);
			}
			else
				effaceFenetre(0 ,0 ,0);
		
			if(difficulte == 0) {
				pageAccueil(fondBlanc);
			}
			else{
				dessineGrille(lignes, colonnes, fondBlanc);
			}		
			break;
			
		case Clavier:
			printf("%c : ASCII %d\n", caractereClavier(), caractereClavier());

			switch (caractereClavier())
			{
				case 'Q': /* Pour sortir quelque peu proprement du programme */
				case 'q':
					termineBoucleEvenements();
					break;

				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; // Changement de mode plein ecran
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'R':
				case 'r':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 20 millisecondes (rapide)
					demandeTemporisation(20);
					break;

				case 'B':
				case 'b':
					fondBlanc = !fondBlanc;
					break;
			}
			break;
			
		case ClavierSpecial:
			break;

		case BoutonSouris:
			switch (etatBoutonSouris())
			{
				case GaucheAppuye:
					int x = abscisseSouris();
            				int y = ordonneeSouris();
            				gererClicGauche(x, y, &lignes, &colonnes, &difficulte, &mines, &cptflag);
            				if(difficulte == 0){
            					Case** tableau = initTableau(lignes, colonnes);
            				}
					afficheGrille(tableau, lignes, colonnes, cptflag);
					break;
				case GaucheRelache:
					break;
				case DroiteAppuye:
				case DroiteRelache:
					break;
				case MilieuAppuye:
				case MilieuRelache:
					break;
				case ScrollDown:
					break;
				case ScrollUp:
					break;
				case ScrollRight:
					break;
				case ScrollLeft:
					break;
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
		
		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			printf("Largeur : %d\t", largeurFenetre());
			printf("Hauteur : %d\n", hauteurFenetre());
			break;
	}
}
void dessineGrille(int lignes, int colonnes, bool fondBlanc)
{
    int largeur = largeurFenetre() - LARGEUR_SCORE;  // Réduire la largeur pour inclure l'espace du score
    int hauteur = hauteurFenetre();
    int largeurCase = largeur / colonnes;
    int hauteurCase = hauteur / lignes;
    int tailleCase = largeurCase < hauteurCase ? largeurCase : hauteurCase;

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            int x = j * tailleCase + 1;
            int y = i * tailleCase + 1;
            if(fondBlanc)
		{
	      		couleurCourante(0, 0, 0);
	      	}
	      	else{
	      		couleurCourante(255, 255, 255);
	      	}
            rectangle(x, y, x + tailleCase - 1, y + tailleCase - 1);
        }
    }
}

void pageAccueil(bool fondBlanc){
	float lFenetre = largeurFenetre();
	float hFenetre = hauteurFenetre();
	epaisseurDeTrait(5);
	
	couleurCourante(0, 255, 0);
        rectangle(lFenetre*3/8, hFenetre*9.5/12, lFenetre*5/8, hFenetre*7.5/12);
        
        couleurCourante(255, 128, 0);
        rectangle(lFenetre*3/8, hFenetre*6.5/12, lFenetre*5/8, hFenetre*4.5/12);
        
        couleurCourante(255, 0, 0);
        rectangle(lFenetre*3/8, hFenetre*3.5/12, lFenetre*5/8, hFenetre*1.5/12);
        
        if(fondBlanc)
        {
      		couleurCourante(0, 0, 0);
      	}
      	else{
      		couleurCourante(255, 255, 255);
      	}
        epaisseurDeTrait(1);
        char titre[] = "DEMINEUR";
        int taillePolice = 38;
        afficheChaine(titre, taillePolice, lFenetre*3/8, hFenetre*10.5/12);
        
	char crédit[] = "Antoine Ferrari  Amelie Phan  Emma Sadak  Marie Richeter";
        taillePolice = 20;
        afficheChaine(crédit, taillePolice, lFenetre*0.5/8, hFenetre*0.5/12);
        
        char facile[] = "FACILE";
        taillePolice = 20;
        afficheChaine(facile, taillePolice, lFenetre*3.5/8, hFenetre*8.5/12);
        
        char moyen[] = "MOYEN";
        taillePolice = 20;
        afficheChaine(moyen, taillePolice, lFenetre*3.5/8, hFenetre*5.5/12);
        
        char difficile[] = "DIFFICILE";
        taillePolice = 20;
        afficheChaine(difficile, taillePolice, lFenetre*3.5/8, hFenetre*2.5/12);
}
