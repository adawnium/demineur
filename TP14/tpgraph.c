#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800
#define HauteurFenetre 600

// Fonction de trace de cercle
void cercle(float centreX, float centreY, float rayon);
/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

void traceGraphique();

void afficheBouton1(int etat);
int gereClicBouton1(int etat);

void afficheBouton2(int cpt);
int gereClicBouton2(int cpt);

void afficheZ();

int saisieEntierGraphique(int oldint);
void afficheEntierGraphique(int entier);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("OpenGL", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}

/* Fonction de trace de cercle */
void cercle(float centreX, float centreY, float rayon)
{
	const int Pas = 20; // Nombre de secteurs pour tracer le cercle
	const double PasAngulaire = 2.*M_PI/Pas;
	int index;
	
	for (index = 0; index < Pas; ++index) // Pour chaque secteur
	{
		const double angle = 2.*M_PI*index/Pas; // on calcule l'angle de depart du secteur
		triangle(centreX, centreY,
				 centreX+rayon*cos(angle), centreY+rayon*sin(angle),
				 centreX+rayon*cos(angle+PasAngulaire), centreY+rayon*sin(angle+PasAngulaire));
			// On trace le secteur a l'aide d'un triangle => approximation d'un cercle
	}
	
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

			traceGraphique();

            afficheBouton1(etatBouton1);

            afficheBouton2(compteur);

            if (printZ)
            {
                afficheZ();
            }
            
            afficheEntierGraphique(entier);
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
                    printZ = !printZ;
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
                    entier = saisieEntierGraphique(entier);
                    break; 
			}
			break;
			
		case ClavierSpecial:
			printf("ASCII %d\n", toucheClavier());
			break;

		case BoutonSouris:
			switch (etatBoutonSouris())
			{
				case GaucheAppuye:
					printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
                    etatBouton1 = gereClicBouton1(etatBouton1);
                    compteur = gereClicBouton2(compteur);
					break;
				case GaucheRelache:
					printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
					break;
				case DroiteAppuye:
				case DroiteRelache:
					puts("Bouton droite");
					break;
				case MilieuAppuye:
				case MilieuRelache:
					puts("Bouton milieu");
					break;
				case ScrollDown:
					puts("Scroll down");
					break;
				case ScrollUp:
					puts("Scroll up");
					break;
				case ScrollRight:
					puts("Scroll right");
					break;
				case ScrollLeft:
					puts("Scroll left");
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

	// Affichage d'une balle magenta
    couleurCourante(255, 0, 255);
    float diametre = lRect / 4.;
    if (!printed)
    {
        printf("Diametre cercle: %.3f\n", diametre);
    }
    cercle(xHG, yHG, diametre / 2.);

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

void afficheBouton1(int etat)
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    // Affichage d'un rectangle "gris moyen"
    couleurCourante(128, 128, 128);

    float factor = 1. / 8.;
    if (!printed)
    {
        printf("Factor: %.3f\n", factor);
    }

    float lRect = lFenetre * factor;
    float hRect = hFenetre * factor;

    float xHG = 0;
    float yHG = hRect;
    float xBD = lRect;
    float yBD = 0;

    rectangle(xHG, yHG, xBD, yBD);

    float xCentreRectangle = xHG + lRect / 2.f;
    float yCentreRectangle = yBD + hRect / 2.f;

        if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    // Texte écrit en blanc    
    epaisseurDeTrait(1);
    int taillePolice = 12;
    float taille = 0, xChaine = 0, yChaine = 0;
    if (etat)
    {
        couleurCourante(255, 0, 0);
        char texte[] = "rouge";
        taille = tailleChaine(texte, taillePolice);
        xChaine = xCentreRectangle - (taille / 2);
        yChaine = yCentreRectangle;
        afficheChaine(texte, taillePolice, xChaine, yChaine);
    }
    else
    {
        couleurCourante(255, 255, 255);
        char texte[] = "gris";
        taille = tailleChaine(texte, taillePolice);
        xChaine = xCentreRectangle - (taille / 2);
        yChaine = yCentreRectangle;
        afficheChaine(texte, taillePolice, xChaine, yChaine);
    }
    if (!printed)
    {
        printf("Chaine [%.3f, %.3f]\n", xChaine, yChaine);
    }

    if (!printed)
    {
        printed = true;
    }
}

int gereClicBouton1(int etat)
{
    int xSouris = abscisseSouris();
    int ySouris = ordonneeSouris();

    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();

    float factor = 1. / 8.;
    // Coordonnées du rectangle "gris moyen"
    float lRect = lFenetre * factor;
    float hRect = hFenetre * factor;

    float xHG = 0;
    float yHG = hRect;
    float xBD = lRect;
    float yBD = 0;

    if (xHG <= xSouris && xSouris <= xBD &&
        yBD <= ySouris && ySouris <= yHG)
    {
        printf("Souris dans rectangle gris moyen\n");
        etat = !etat;
    }

    return etat;
}

void afficheBouton2(int cpt)
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    // Affichage d'un rectangle "gris foncé"
    couleurCourante(96, 96, 96);

    float factor = 1. / 8.;
    if (!printed)
    {
        printf("Factor: %.3f\n", factor);
    }

    float lRect = lFenetre * factor;
    float hRect = hFenetre * factor;

    float xHG = lFenetre - lRect;
    float yHG = hFenetre;
    float xBD = lFenetre;
    float yBD = hFenetre - hRect;
    float xCentreRectangle = xHG + lRect / 2.f;
    float yCentreRectangle = yBD + hRect / 2.f;
    rectangle(xHG, yHG, xBD, yBD);

    rectangle(xHG, yHG, xBD, yBD);

    if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    // Texte écrit en blanc    
    epaisseurDeTrait(1);
    int taillePolice = 12;
    float taille = 0, xChaine = 0, yChaine = 0;
    couleurCourante(255, 255, 255);
    char texte[15];
    // Permet de formater dans une chaine de caractère
    // l'argument passé, soit ici, un entier.
    sprintf(texte, "%d", cpt);
    taille = tailleChaine(texte, taillePolice);
    xChaine = xCentreRectangle - (taille / 2);
    yChaine = yCentreRectangle;
    afficheChaine(texte, taillePolice, xChaine, yChaine);

    if (!printed)
    {
        printf("Chaine [%.3f, %.3f]\n", xChaine, yChaine);
    }

    if (!printed)
    {
        printed = true;
    }
}

int gereClicBouton2(int cpt)
{
    int xSouris = abscisseSouris();
    int ySouris = ordonneeSouris();

    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();

    float factor = 1. / 8.;
    // Coordonnées du rectangle "gris moyen"
    float lRect = lFenetre * factor;
    float hRect = hFenetre * factor;

    float xHG = lFenetre - lRect;
    float yHG = hFenetre;
    float xBD = lFenetre;
    float yBD = hFenetre - hRect;

    if (xHG <= xSouris && xSouris <= xBD &&
        yBD <= ySouris && ySouris <= yHG)
    {
        printf("Souris dans rectangle gris foncé\n");
        ++cpt;
    }

    return cpt;
}

void afficheZ()
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    int xCentreFenetre = lFenetre / 2;
    int yCentreFenetre = hFenetre / 2;
    // Affichage en rouge
    couleurCourante(255, 0, 0);
    epaisseurDeTrait(10);


    float factor = 1. / 2.;
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

    ligne(xHG, yHG, xBD, yHG);
    ligne(xBD, yHG, xHG, yBD);
    ligne(xHG, yBD, xBD, yBD);

    if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    if (!printed)
    {
        printed = true;
    }
}

int saisieEntierGraphique(int oldint)
{
    char c = caractereClavier();
    
    char texte[50] = { 0 };
    snprintf(texte, sizeof(texte), "%d%c", oldint, c);
    
    oldint = strtol(texte, NULL, 10);
    return oldint;
}

void afficheEntierGraphique(int entier)
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    int xCentreFenetre = lFenetre / 2;
    int yCentreFenetre = hFenetre / 2;
    // Texte écrit en blanc    
    epaisseurDeTrait(1);
    couleurCourante(255, 0, 0);
    char texte[50] = {0};
    snprintf(texte, sizeof (texte), "%d", entier);
    int taillePolice = 20;
    float taille = tailleChaine(texte, taillePolice);
    float xChaine = xCentreFenetre - (taille / 2);
    float yChaine = yCentreFenetre;
    afficheChaine(texte, taillePolice, xChaine, yChaine);
    if (!printed)
    {
        printf("Chaine [%.3f, %.3f]\n", xChaine, yChaine);
    }

    if (!printed)
    {
        printed = true;
    }
}