#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h>	// Pour pouvoir utiliser printf()
#include <stdbool.h>
#include <time.h>
#include "grille.h"
#include "GFXLIB/GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "affichage.h"
#include "utilisateur.h"

#define LargeurFenetre 800 // Augmenté pour inclure l'espace du score
#define HauteurFenetre 601
#define LARGEUR_SCORE 400

void gererClicGauche(int x, int y, int *lignes, int *colonnes, int *difficulte, int *mines, int *cptflag, bool *dehors, bool *firstclick, Case **tableau, bool *minetouche, bool *win, bool *initialise, time_t *startTime, bool *powerstock, bool *poweruse, bool *relancement)
{
	float lFenetre = largeurFenetre();
	float hFenetre = hauteurFenetre();

	if (*difficulte != 0 && !*win && !*minetouche) // si on est sur la page de jeu qui est en cours
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
			if (*firstclick == false)                                                               //pour le premier clic qui initialise la grille
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
				*startTime = time(NULL); //démarer le chrono
				*firstclick = true;
			}
			else
			{
				if (!tableau[ligneCliquee][colonneCliquee].hasMine && !tableau[ligneCliquee][colonneCliquee].flagged) //si une case normal est clique
				{
					bool caseselected = true;
					reveleCase(tableau, ligneCliquee, colonneCliquee, *lignes, *colonnes, caseselected);
					if (victoire(tableau, *lignes, *colonnes, *mines)) //verifie si le joueur a gagné
					{
						*win = true;
					}
				}
				else if (!tableau[ligneCliquee][colonneCliquee].revealed && !tableau[ligneCliquee][colonneCliquee].flagged) //si le joueur touche une mine
				{
					if(*poweruse){                //esquive la défaite si le pouvoir est activé
						*poweruse= false;
					}else{ *minetouche = true; }
					tableau[ligneCliquee][colonneCliquee].etat = 'M';
					tableau[ligneCliquee][colonneCliquee].revealed = true;
					*cptflag -= 1;
				}
			}
		}

		else
		{
			*dehors = true;
		}
		float rectangles[3][4] = {                                                           // Créer un tableau pour simplifier la condition qui suit
			{lFenetre * 5 / 8, hFenetre * 7 / 12, lFenetre * 7 / 8, hFenetre * 5.5 / 12},
			{lFenetre * 5 / 8, hFenetre * 4.5 / 12, lFenetre * 7 / 8, hFenetre * 3 / 12},
			{lFenetre * 5 / 8, hFenetre * 2 / 12, lFenetre * 7 / 8, hFenetre * 0.5 / 12}};

		for (int i = 0; i < 3; i++)
		{
			if (x >= rectangles[i][0] && x <= rectangles[i][2] && y >= rectangles[i][3] && y <= rectangles[i][1])  // Verifie si l'utilisateur a cliqué sur un des trois boutons à droite de la grille
			{
				switch (i)
				{
				case 0:
					// Recommencer
					libererGrille(tableau, *lignes);
					*initialise = false;
					*firstclick = false;
					*cptflag = *mines;
					*powerstock = true;
					*poweruse = false;
					*relancement = true;
					printf("Vous avez recommencé\n");
					break;
				case 1:
					// niveau suivant

					switch (*difficulte)
					{
					case 1: // facile -> moyen on remet tous à 0 sauf la difficulté bien évidement
						*difficulte = 2;
						*lignes = 0;
						*colonnes = 0;
						*mines = 0;
						*cptflag = 0;
						libererGrille(tableau, *lignes);
						choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
						*initialise = false;
						*firstclick = false;
						*powerstock = true;
						*poweruse = false;
						*relancement = true;
						if(lFenetre < 857 || hFenetre < (float) HauteurFenetre) // condition pour qu'au changement la grille soit la taille de la fenetre soit au minimum pour un meilleur rendu 
						{
							redimensionneFenetre(857, HauteurFenetre);
						}
						break;
					case 2:  // moyen -> difficile
						*difficulte = 3;
						*lignes = 0;
						*colonnes = 0;
						*mines = 0;
						*cptflag = 0;
						libererGrille(tableau, *lignes);
						choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
						*initialise = false;
						*firstclick = false;
						*powerstock = true;
						*poweruse = false;
						*relancement = true;
						if(lFenetre < 882 || hFenetre < (float) HauteurFenetre)
						{
							redimensionneFenetre(882, HauteurFenetre);
						}
						
						break;
					case 3: // difficile -> facile
						*difficulte = 1;
						*lignes = 0;
						*colonnes = 0;
						*mines = 0;
						*cptflag = 0;
						libererGrille(tableau, *lignes);
						choixDifficulte(*difficulte, lignes, colonnes, mines, cptflag);
						*initialise = false;
						*firstclick = false;
						*powerstock = true;
						*poweruse = false;
						*relancement = true;
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
					*powerstock = true;
					*poweruse = false;
					*relancement = true;
					libererGrille(tableau, *lignes);
					break;
				}
			}
		}
	}

	if (*difficulte == 0) // si on est sur la page d'accueil
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

void gererClicDroit(int x, int y, int lignes, int colonnes, int difficulte, Case **tableau, int *cptflag)
{
	if (difficulte != 0)
	{
		int largeur = largeurFenetre() - LARGEUR_SCORE; // Réduire la largeur pour inclure l'espace du score
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
			if (tableau[ligneCliquee][colonneCliquee].flagged == false)
			{
				placerDrapeau(tableau, ligneCliquee, colonneCliquee, cptflag);
			}
			else
			{
				enleverDrapeau(tableau, ligneCliquee, colonneCliquee, cptflag);
			}
		}
	}
}

void gestionEvenement(EvenementGfx evenement)
{
    static bool pleinEcran = false;
    static bool fondBlanc = true;
    static int difficulte = 0;
    static int lignes = 0;
    static int colonnes = 0;
    static int mines = 0;
    static int cptflag = 0;
    static Case **tableau = NULL;
    static bool initialise = false;
    static bool firstclick = false;
    bool dehors = false;
    static bool minetouche = false;
    static bool win = false;
    static time_t startTime = 0;
    static double elapsedTime = 0;
    static bool relancement = false;
    static bool powerstock = true;
    static bool poweruse = false;

    switch (evenement)
    {
    case Initialisation:
    		/* Le message "Initialisation" est envoye une seule fois, au debut du
		programme : il permet de fixer "image" a la valeur qu'il devra conserver
		jusqu'a la fin du programme : soit "image" reste a NULL si l'image n'a
		pas pu etre lue, soit "image" pointera sur une structure contenant
		les caracteristiques de l'image "imageNB.bmp" */
		
		
		// Configure le systeme pour generer un message Temporisation
		// toutes les 20 millisecondes
        demandeTemporisation(20);
        break;
    case Temporisation:
    			// il faut redessiner la fenetre
        rafraichisFenetre();
        break;
    case Affichage:
        if (fondBlanc)
        {				// On part d'un fond d'ecran blanc
            effaceFenetre(255, 255, 255);
        }
        else
        {				// On part d'un fond d'ecran noir
            effaceFenetre(0, 0, 0);
        }
        if (difficulte == 0)
        {
            pageAccueil(fondBlanc);
        }
        else
        {
            dessineGrille(lignes, colonnes, fondBlanc, win, minetouche, tableau, cptflag, startTime, elapsedTime, firstclick, poweruse, relancement);
        }
        break;
    case Clavier:
        switch (caractereClavier())
        {
        case 'Q':
        case 'q':
            termineBoucleEvenements();
            break;
        case 'F':
        case 'f':
            pleinEcran = !pleinEcran;
            if (pleinEcran)
            {
                modePleinEcran();
            }
            else
            {
                redimensionneFenetre(LargeurFenetre, HauteurFenetre);
            }
            break;
        case 'R':
        case 'r':
            demandeTemporisation(20);
            break;
        case 'B':
        case 'b':
            fondBlanc = !fondBlanc;
            break;

        case 'E':
        case 'e':
            if (powerstock)          // active le pouvoir secret ...
            {
                poweruse = true;
                powerstock = false;
            }
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
            int y = ordonneeSouris();  // récupère les coordonnées du clic
            if (relancement)
            {
                relancement = !relancement;
            }
            if ((win || minetouche) && x >= largeurFenetre() * 1.5 / 8 && x <= largeurFenetre() * 3.5 / 8 && y >= hauteurFenetre() * 4 / 12 && y <= hauteurFenetre() * 6 / 12)
            {
                // Rejouer
                firstclick = false;
                minetouche = false;
                win = false;
                libererGrille(tableau, lignes);
                tableau = initTableau(lignes, colonnes);
                cptflag = mines;
                afficheGrille(tableau, lignes, colonnes, cptflag);
                relancement = true;
                powerstock = true;
                poweruse = false;
            }
            else if ((win || minetouche) && x >= largeurFenetre() * 4.5 / 8 && x <= largeurFenetre() * 6.5 / 8 && y >= hauteurFenetre() * 4 / 12 && y <= hauteurFenetre() * 6 / 12)
            {
                // Quitter
                libererGrille(tableau, lignes);
                termineBoucleEvenements();
            }
            if (!minetouche && !win && !relancement)  // si le jeu n'est pas fini
            {
                gererClicGauche(x, y, &lignes, &colonnes, &difficulte, &mines, &cptflag, &dehors, &firstclick, tableau, &minetouche, &win, &initialise, &startTime, &powerstock, &poweruse, &relancement);
                if (difficulte != 0 && !initialise)  // initialise un fois la difficulté choisi sur l'écran d'acceuil
                {
                    tableau = initTableau(lignes, colonnes);
                    initialise = !initialise;
                }
                if (initialise && !dehors && !minetouche)
                {
                    afficheGrille(tableau, lignes, colonnes, cptflag); //affiche la grille sur la console
                }
                if (minetouche)
                {
                    printf("TU AS PERDU\n\n");
                    afficheGrilleComplete(tableau, lignes, colonnes); // s'affiche en cas de défaite sur la console
                }
                if (win)
                {
                    printf("TU AS GAGNE\n\n");
                    afficheGrilleComplete(tableau, lignes, colonnes); // s'affiche en cas de victoire sur la console
                }
            }
            break;
        case GaucheRelache:
            break;
        case DroiteAppuye:
            if (!minetouche && firstclick)
            {
                int x = abscisseSouris();
                int y = ordonneeSouris();
                gererClicDroit(x, y, lignes, colonnes, difficulte, tableau, &cptflag); //place et enlève les drapeeaux
                afficheGrille(tableau, lignes, colonnes, cptflag);
            }
            break;
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
    case Redimensionnement:
        printf("Largeur : %d\t", largeurFenetre());
        printf("Hauteur : %d\n", hauteurFenetre());
        break;
    }
}
