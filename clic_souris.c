#include <stdio.h>
#include "GFXLIB/GfxLib.h"
#include <stdlib.h>

int main(void) {
    int clic_gauche = 0;
    int clic_droit = 0;
    int case_1 = 0;
    int case_2 = 0;
    int case_3 = 0;
    int case_vide = 0;
    int case_non_cliquee = 0;
    int mine = 0;
    int balise = 0;
    int placed = 0;
}

void clic_gauche (*case_1, *case_2, *case_3, *case_vide)
{
    switch {
        case_1 = placed;
        case_2 = placed;
        case_3 = placed;
        case_vide = placed;
        mine = placed;
    }

    int rand(placed);
}

void clic_droit (*balise)
{
    case_non_cliquee = balise;
}

//////////////////////////////////////////////////////////

#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800
#define HauteurFenetre 600

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("OpenGL", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou pas
    static int etatBouton1 = 0;
    static int compteur = 0;
    static bool fond = false;
    static bool printZ = false;
    static int entier = 0;
	
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
			// Les coordonnees de la balle ayant eventuellement change,
			// il faut redessiner la fenetre :
			rafraichisFenetre();
			break;
			
		case Affichage:
            if (fond)
            {
                // On part d'un fond d'ecran blanc
			    effaceFenetre(255, 255, 255);
            }
            else
            {
                // On part d'un fond d'ecran noir
			    effaceFenetre(0, 0, 0);
            }

            if (printZ)
            {
                afficheZ();
            }
            
            afficheEntierGraphique(entier);
			break;

		case BoutonSouris:
			switch (etatBoutonSouris())
			{
				case GaucheAppuye:
					
					break;
				case DroiteAppuye:
                    case_non_cliquee = balise;
					break;
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
		
		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			// Donc le systeme nous en informe
			printf("Largeur : %d\t", largeurFenetre());
			printf("Hauteur : %d\n", hauteurFenetre());
			break;
	}
}
