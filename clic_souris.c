// #include <stdio.h>
// #include "GFXLIB/GfxLib.h"
// #include <stdlib.h>

// int main(void) {
//     int clic_gauche = 0;
//     int clic_droit = 0;
//     int case_1 = 0;
//     int case_2 = 0;
//     int case_3 = 0;
//     int case_vide = 0;
//     int case_non_cliquee = 0;
//     int mine = 0;
//     int balise = 0;
//     int placed = 0;
// }

// void clic_gauche (*case_1, *case_2, *case_3, *case_vide)
// {
//     switch {
//         case_1 = placed;
//         case_2 = placed;
//         case_3 = placed;
//         case_vide = placed;
//         mine = placed;
//     }

//     int rand(placed);
// }

// void clic_droit (*balise)
// {
//     case_non_cliquee = balise;
// }

//////////////////////////////////////////////////////////

#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

#define LargeurFenetre 800
#define HauteurFenetre 600

void gestionEvenement(EvenementGfx evenement);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("OpenGL", LargeurFenetre, HauteurFenetre);
	
	lanceBoucleEvenements();
	
	return 0;
}

void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false;
    static int etatBouton1 = 0;
    static int compteur = 0;
    static bool fond = false;
    static bool printZ = false;
    static int entier = 0;
	
	switch (evenement)
	{
		case Initialisation:
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			rafraichisFenetre();
			break;
			
		case Affichage:
            if (fond)
            {
				effaceFenetre(255, 255, 255);
            }
            else
            {
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

					break;
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
	}
}
