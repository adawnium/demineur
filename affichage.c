#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GFXLIB/GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "GFXLIB/BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "GFXLIB/ESLib.h" // Pour utiliser valeurAleatoire()

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800
#define HauteurFenetre 600

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

void traceGraphique();

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

			traceGraphique();
			
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

				case 'L':
				case 'l':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 100 millisecondes (lent)
					demandeTemporisation(100);
					break;

				case 'S':
				case 's':
					// Configure le systeme pour ne plus generer de message Temporisation
					demandeTemporisation(-1);
					break;

                case 'b':
                    fond = !fond;
                    break;

                case 'z':
                    break;

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    break; 
			}
			break;
			

		case BoutonSouris:
			switch (etatBoutonSouris())
			{
				case GaucheAppuye:
					printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
					reveleCase(); //TODO : Fonction qui revele la case cliquée
					break;
				case GaucheRelache:
					printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
					deposeDrapeau(); //TODO : Fonction qui depose un drapeau
					break;
				case DroiteAppuye:
				case DroiteRelache:
					puts("Bouton droite");
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

void traceGraphique()
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.    
    static bool printed = false;

    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();

    int xCentreFenetre = lFenetre / 2;
    int yCentreFenetre = hFenetre / 2;
    
    // Affichage d'une ligne verte
    couleurCourante(0, 255, 0);
    epaisseurDeTrait(5);
    ligne(0, hFenetre, lFenetre, 0);

    // Affichage d'un rectangle "cyan"
    couleurCourante(0, 255, 255);
    float factor = sqrt(1. / 3.);
    if (!printed)
    {
        printf("Factor: %.3f\n", factor);
    }

    float lRect = lFenetre * factor;
    float hRect = hFenetre * factor;

    float xHG = xCentreFenetre - lRect / 2.f;
    float yHG = yCentreFenetre + hRect / 2.f;
    float xBD = xCentreFenetre + lRect / 2.f;
    float yBD = yCentreFenetre - hRect / 2.f;
    rectangle(xHG, yHG, xBD, yBD);

	// Affichage de 10 points blancs
    couleurCourante(255, 255, 255);
    float yPoints = (yHG + yBD) / 2;
    for (float x = xHG; x <= xBD; x += lRect / 9.)
    {
        if (!printed)
        {
            printf("Points[%.3f, %.3f]\n", x, yPoints);
        }
        point(x, yPoints);
    }

    // Rectangle bleu
    couleurCourante(0, 0, 255);
    factor = sqrt(1. / 8.);
    lRect = lFenetre * factor;
    hRect = hFenetre * factor;


    xHG = 0;
    yHG = hFenetre;
    xBD = lRect;
    yBD = hFenetre - hRect;

    rectangle(xHG, yHG, xBD, yBD);

    float xCentreRectangle = xHG + lRect / 2.f;
    float yCentreRectangle = yBD + hRect / 2.f;
    
    // Texte écrit en blanc
    couleurCourante(255, 255, 255);
    epaisseurDeTrait(1);
    char texte[] = "Etudiant";
    int taillePolice = 12;
    float taille = tailleChaine(texte, taillePolice);
    float xChaine = xCentreRectangle - (taille / 2);
    float yChaine = yCentreRectangle - (taillePolice / 2);
    if (!printed)
    {
        printf("Chaine [%.3f, %.3f]\n", xChaine, yChaine);
    }
    afficheChaine(texte, taillePolice, xChaine, yChaine);

    if (!printed)
    {
        printed = true;
    }

}

void reveleCase(int** grille) {
	if(etatBoutonSouris() != GaucheAppuye) {
		printf("Autre\n");
		return;
	}

	if(grille[i][j])
}