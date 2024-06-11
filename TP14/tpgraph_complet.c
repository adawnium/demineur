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

int main(int argc, char **argv)
{
    initialiseGfx(argc, argv);

    prepareFenetreGraphique("GfxLib", LargeurFenetre, HauteurFenetre);

    /* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
            qui elle-meme utilise fonctionAffichage ci-dessous */
    lanceBoucleEvenements();

    return 0;
}

/* Fonction de trace de cercle */
void cercle(float centreX, float centreY, float rayon)
{
    const int Pas = 20; // Nombre de secteurs pour tracer le cercle
    const double PasAngulaire = 2. * M_PI / Pas;
    int index;

    for (index = 0; index < Pas; ++index) // Pour chaque secteur
    {
        const double angle = 2. * M_PI * index / Pas; // on calcule l'angle de depart du secteur
        triangle(centreX, centreY,
                 centreX + rayon * cos(angle), centreY + rayon * sin(angle),
                 centreX + rayon * cos(angle + PasAngulaire), centreY + rayon * sin(angle + PasAngulaire));
        // On trace le secteur a l'aide d'un triangle => approximation d'un cercle
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

    if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    // Affichage d'un disque magenta
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
    float xCentreRectangle = xHG + lRect / 2.f;
    float yCentreRectangle = yBD + hRect / 2.f;
    rectangle(xHG, yHG, xBD, yBD);

    // Texte écrit en blanc
    couleurCourante(255, 255, 255);
    epaisseurDeTrait(1);
    char texte[] = "Sebastien";
    int taillePolice = 12;
    float taille = tailleChaine(texte, taillePolice);
    float xChaine = xCentreRectangle - (taille / 2);
    float yChaine = yCentreRectangle;
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
    // Affichage d'un rectangle "cyan"
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
    float xCentreRectangle = xHG + lRect / 2.f;
    float yCentreRectangle = yBD + hRect / 2.f;
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

void afficheBouton2(int cpt)
{
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    // Affichage d'un rectangle "cyan"
    couleurCourante(64, 64, 64);
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

    if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    // Texte écrit en blanc    
    epaisseurDeTrait(1);
    couleurCourante(255, 255, 255);
    char texte[50] = {0};
    snprintf(texte, sizeof (texte), "%d", cpt);
    int taillePolice = 12;
    float taille = tailleChaine(texte, taillePolice);
    float xChaine = xCentreRectangle - (taille / 2);
    float yChaine = yCentreRectangle;
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
    // Variable permettant de gérer l'affichage d'informations
    // complémentaires dans le terminal.
    static bool printed = false;
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    // Affichage d'un rectangle "cyan"
    couleurCourante(64, 64, 64);
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

    if (!printed)
    {
        printf("Rectangle L:%.3f,H:%.3f\n", lRect, hRect);
        printf("PointHG[%.3f, %.3f], PointsBD[%.3f, %.3f]\n",
               xHG, yHG, xBD, yBD);
    }

    int xSouris = abscisseSouris();
    int ySouris = ordonneeSouris();

    if (xHG <= xSouris && xSouris <= xBD && yBD <= ySouris && ySouris <= yHG)
    {
        // Souris à l'intérieur du rectangle
        ++cpt;
    }


    if (!printed)
    {
        printed = true;
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

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
    static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou pas
    static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran

    /* On va aussi animer une balle traversant l'ecran */
    static int xBalle;
    static int yBalle;
    static int vxBalle = 7;
    static int vyBalle = -7;
    static int etatBouton1 = 0;
    static int cptBouton2 = 0;
    static int fond = 0;
    static int printZ = 0;
    static int entier = 0;

    switch (evenement)
    {
    case Initialisation:
        xBalle = largeurFenetre() * valeurAleatoire();
        yBalle = hauteurFenetre() * valeurAleatoire();
        image = lisBMPRGB("ISEN.bmp");
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
        // On met a jour les coordonnees de la balle
        xBalle += vxBalle;
        yBalle += vyBalle;

        // On fait rebondir la balle si necessaire
        if (xBalle < 0 || xBalle >= largeurFenetre())
            vxBalle = -vxBalle;
        if (yBalle < 0 || yBalle >= hauteurFenetre())
            vyBalle = -vyBalle;
        // Les coordonnees de la balle ayant eventuellement change,
        // il faut redessiner la fenetre :
        rafraichisFenetre();
        break;

    case Affichage:

        // On part d'un fond d'ecran blanc
        if (fond)
        {
            effaceFenetre(0, 0, 0);
        }
        else
        {
            effaceFenetre(255, 255, 255);
        }

        traceGraphique();

        afficheBouton1(etatBouton1);
        afficheBouton2(cptBouton2);

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
            libereDonneesImageRGB(&image); /* On libere la structure image,
					c'est plus propre, meme si on va sortir du programme juste apres */
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
        if (etatBoutonSouris() == GaucheAppuye)
        {
            printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
            // Si le bouton gauche de la souris est appuye, faire repartir
            // la balle de la souris
            xBalle = abscisseSouris();
            yBalle = ordonneeSouris();

            etatBouton1 = !etatBouton1;
        }
        else if (etatBoutonSouris() == GaucheRelache)
        {
            printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
        }
        else if (etatBoutonSouris() == DroiteAppuye)
        {
            printf("Bouton droite appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
            cptBouton2 = gereClicBouton2(cptBouton2);
        }
        break;

    case Souris: // Si la souris est deplacee
        break;

    case Inactivite: // Quand aucun message n'est disponible
        break;

    case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
        // Donc le systeme nous en informe
        if (xBalle >= largeurFenetre())
            xBalle = largeurFenetre() - 1;
        if (yBalle >= hauteurFenetre())
            yBalle = hauteurFenetre() - 1;
        printf("Largeur : %d\t", largeurFenetre());
        printf("Hauteur : %d\n", hauteurFenetre());
        break;
    }
}
