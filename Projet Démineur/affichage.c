#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h>  // Pour pouvoir utiliser printf()
#include <stdbool.h>
#include <time.h>
#include "grille.h"
#include "GFXLIB/GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "affichage.h"

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800 // Augmenté pour inclure l'espace du score
#define HauteurFenetre 601
#define LARGEUR_SCORE 400

void pageJeu(int lignes, int colonnes, bool fondBlanc, bool win, bool minetouche, Case **tableau, int cptflag, time_t startTime, double elapsedTime, bool firstclick, bool poweruse, bool relancement)
{
    int largeur = largeurFenetre();
    int largeurgrille = largeurFenetre() - LARGEUR_SCORE;
    int hauteur = hauteurFenetre();
    int largeurCase = largeurgrille / colonnes;
    int hauteurCase = hauteur / lignes;
    int tailleCase = largeurCase < hauteurCase ? largeurCase : hauteurCase;

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            // Inversion des coordonnées pour le dessin des cases
            int x = j * tailleCase + 1;
            int y = (lignes - 1 - i) * tailleCase + 1;

            // Si le joueur a touché une mine, on révèle toutes les cases
            if (minetouche)
            {
                tableau[i][j].revealed = true;
            }

            if (tableau[i][j].revealed)
            {
                // Couleur pour les cases révélées
                couleurCourante(247, 238, 255); // Violet très clair
                rectangle(x, y, x + tailleCase - 1, y + tailleCase - 1);

                // Si la case contient une mine
                if (tableau[i][j].hasMine)
                {
                    couleurCourante(0, 0, 0); // Noir pour la mine
                    int taillePolice = tailleCase / 2;
                    afficheChaine("M", taillePolice, x + tailleCase / 4, y + tailleCase / 4);
                }
                else
                {
                    // Afficher le nombre de mines adjacentes
                    int minesAdjacentes = tableau[i][j].nbMinesAdj;
                    if (minesAdjacentes > 0)
                    {
                        switch (minesAdjacentes)
                        {
                        case 1:
                            couleurCourante(0, 0, 255);
                            break; // Bleu
                        case 2:
                            couleurCourante(0, 255, 0);
                            break; // Vert
                        case 3:
                            couleurCourante(255, 0, 0);
                            break; // Rouge
                        case 4:
                            couleurCourante(0, 0, 128);
                            break; // Bleu foncé
                        case 5:
                            couleurCourante(128, 0, 0);
                            break; // Rouge foncé
                        case 6:
                            couleurCourante(0, 128, 128);
                            break; // Cyan foncé
                        default:
                            couleurCourante(0, 0, 0);
                            break; // Noir pour 7 et plus
                        }

                        char nombre[2];
                        sprintf(nombre, "%d", minesAdjacentes);
                        int taillePolice = tailleCase / 2;
                        afficheChaine(nombre, taillePolice, x + tailleCase / 4, y + tailleCase / 4);
                    }
                }
            }
            else
            {
                couleurCourante(213, 200, 225); // Violet
                rectangle(x, y, x + tailleCase - 1, y + tailleCase - 1);

                // Si la case a un drapeau
                if (tableau[i][j].flagged)
                {
                    couleurCourante(155, 103, 203); // Violet pour le drapeau
                    rectangle(x, y, x + tailleCase - 1, y + tailleCase - 1);
                }
            }
        }
    }

    if (fondBlanc) //inverse les couleur pour plus de lisibilité
    {
        couleurCourante(0, 0, 0);
    }
    else
    {
        couleurCourante(255, 255, 255);
    }

    int taillePolice = largeur / 25;
    epaisseurDeTrait(3);
    char titre[] = "DEMINEUR";           // pour afficher le titre du jeu
    couleurCourante(87, 9, 135);
    afficheChaine(titre, taillePolice, largeur * 0.645, hauteur * 0.90);

    epaisseurDeTrait(2);
    int x1 = largeur*7/8, x2 = largeur*7.5/8, y1 = hauteur*11.5/12, y2 = hauteur*10.5/12;  // pour afficher le rectangle vide pour le pouvoir
    ligne(x1, y1, x2, y1);
    ligne(x1, y1, x1, y2);
    ligne(x2, y1, x2, y2);
    ligne(x1, y2, x2, y2);
    if (poweruse)
    {
        rectangle(x1, y1, x2, y2); //le rectangle se rempli pour montrer que le pouvoir est activé
    }

    taillePolice = largeur / 35;
    char chrono[20];
    static double finalTime = 0;

    if (firstclick && !win && !minetouche && !relancement)
    {
        time_t currentTime;
        time(&currentTime);
        elapsedTime = difftime(currentTime, startTime); //calcule la différence de "temps"
        finalTime = elapsedTime; // Mettre à jour finalTime pendant le jeu
    }
    else if ((win || minetouche) && finalTime == 0 && !relancement)
    {
        finalTime = elapsedTime; // Bloquer finalTime à la fin du jeu
    }
    else if (relancement)
    {
        finalTime = 0; //remets le chrono à 0 quand il n'y a plus de partie en cours
    }

    epaisseurDeTrait(1);
    if (fondBlanc)
    {
        couleurCourante(0, 0, 0);
    }
    else
    {
        couleurCourante(255, 255, 255);
    }
    sprintf(chrono, "Chronometre: %.0f s", finalTime);
    afficheChaine(chrono, taillePolice, largeur * 0.618, hauteur * 9.4 / 12);

    epaisseurDeTrait(1);
    char flag[50];
    sprintf(flag, "Mines restantes: %2d", cptflag);
    afficheChaine(flag, taillePolice, largeur * 0.612, hauteur * 8.1 / 12); // compteur de flag

    couleurCourante(74, 35, 90);
    rectangle(largeur * 5 / 8, hauteur * 7 / 12, largeur * 7 / 8, hauteur * 5.5 / 12); //début de création des 3 boutons sur le coté
    couleurCourante(125, 60, 152);
    rectangle(largeur * 5 / 8, hauteur * 4.5 / 12, largeur * 7 / 8, hauteur * 3 / 12);
    couleurCourante(187, 143, 206);
    rectangle(largeur * 5 / 8, hauteur * 2 / 12, largeur * 7 / 8, hauteur * 0.5 / 12);

    couleurCourante(255, 255, 255);
    taillePolice = largeur / 35;
    char recommencer[] = "Recommencer";
    afficheChaine(recommencer, taillePolice, largeur * 0.649, hauteur * 0.51);
    taillePolice = largeur / 40;
    char EcranTitre[] = "Ecran d'accueil";
    afficheChaine(EcranTitre, taillePolice, largeur * 0.655, hauteur * 0.095);
    taillePolice = largeur / 47;
    char switchNiveau[] = "Changer de niveau";
    afficheChaine(switchNiveau, taillePolice, largeur * 0.644, hauteur * 0.305);  //fin de création des 3 boutons sur le coté

    if (win || minetouche)
    {
        if (fondBlanc)
        {
            couleurCourante(255, 255, 255);
        }
        else
        {
            couleurCourante(0, 0, 0);
        }
        // Dessiner le rectangle principal
        int rectX1 = largeur * 1 / 8;
        int rectY1 = hauteur * 9 / 12;
        int rectX2 = largeur * 7 / 8;
        int rectY2 = hauteur * 3 / 12;
        rectangle(rectX1, rectY1, rectX2, rectY2);

        // Surligner les bords du rectangle
        epaisseurDeTrait(3);
        couleurCourante(158, 53, 255); // Rouge pour les lignes surlignées

        // Ligne supérieure
        ligne(rectX1 - 2, rectY1 + 2, rectX2 + 2, rectY1 + 2);
        ligne(rectX1 - 1, rectY1 + 1, rectX2 + 1, rectY1 + 1);

        // Ligne inférieure
        ligne(rectX1 - 2, rectY2 - 2, rectX2 + 2, rectY2 - 2);
        ligne(rectX1 - 1, rectY2 - 1, rectX2 + 1, rectY2 - 1);

        // Ligne gauche
        ligne(rectX1 - 2, rectY1 + 2, rectX1 - 2, rectY2 - 2);
        ligne(rectX1 - 1, rectY1 + 1, rectX1 - 1, rectY2 - 1);

        // Ligne droite
        ligne(rectX2 + 2, rectY1 + 2, rectX2 + 2, rectY2 - 2);
        ligne(rectX2 + 1, rectY1 + 1, rectX2 + 1, rectY2 - 1);

        epaisseurDeTrait(1); // Peut-être rajouter des lignes pour que ça se voit plus
        if (fondBlanc)
        {
            couleurCourante(0, 0, 0);
        }
        else
        {
            couleurCourante(255, 255, 255);
        }
        rectangle(largeur * 1.5 / 8, hauteur * 6 / 12, largeur * 3.5 / 8, hauteur * 4 / 12);
        rectangle(largeur * 4.5 / 8, hauteur * 6 / 12, largeur * 6.5 / 8, hauteur * 4 / 12); // les deux boutons rejouer et quitter
        taillePolice = largeur / 23;
        if (win)
        {
            char gagne[] = "TU AS GAGNE";
            afficheChaine(gagne, taillePolice, largeur * 0.33, hauteur * 7.5 / 12);
        }
        if (minetouche)
        {
            char perdu[] = "TU AS PERDU";
            afficheChaine(perdu, taillePolice, largeur * 0.33, hauteur * 7.5 / 12);
        }
        if (fondBlanc)
        {
            couleurCourante(255, 255, 255);
        }
        else
        {
            couleurCourante(0, 0, 0);
        }
        taillePolice = largeur / 30;
        char rejouer[] = "Rejouer";
        afficheChaine(rejouer, taillePolice, largeur * 0.247, hauteur * 0.4);
        char quitter[] = "Quitter";
        afficheChaine(quitter, taillePolice, largeur * 0.635, hauteur * 0.4);
    }
}

void pageAccueil(bool fondBlanc)
{
    float lFenetre = largeurFenetre();
    float hFenetre = hauteurFenetre();
    epaisseurDeTrait(5);

    couleurCourante(46, 204, 113);
    rectangle(lFenetre * 3 / 8, hFenetre * 9.5 / 12, lFenetre * 5 / 8, hFenetre * 7.5 / 12);  // les trois boutons pour sélectionner la difficulté

    couleurCourante(243, 156, 18);
    rectangle(lFenetre * 3 / 8, hFenetre * 6.5 / 12, lFenetre * 5 / 8, hFenetre * 4.5 / 12);

    couleurCourante(192, 57, 43);
    rectangle(lFenetre * 3 / 8, hFenetre * 3.5 / 12, lFenetre * 5 / 8, hFenetre * 1.5 / 12);

    if (fondBlanc)
    {
        couleurCourante(0, 0, 0);
    }
    else
    {
        couleurCourante(255, 255, 255);
    }
    epaisseurDeTrait(3);
    char titre[] = "DEMINEUR";
    int taillePolice = lFenetre / 16;
    afficheChaine(titre, taillePolice, lFenetre * 0.337, hFenetre * 0.875);   // affiche les différentes chaines de caractère de manière responsive

    epaisseurDeTrait(2);
    ligne(lFenetre * 0.333, hFenetre * 0.865, lFenetre * 0.67, hFenetre * 0.867);

    epaisseurDeTrait(1);

    char crédit[] = "Antoine Ferrari  Amelie Phan  Emma Sadak  Marie Richeter";
    taillePolice = lFenetre / 40;
    afficheChaine(crédit, taillePolice, lFenetre * 0.07, hFenetre * 0.05);

    char facile[] = "FACILE";
    taillePolice = lFenetre / 30;
    afficheChaine(facile, taillePolice, lFenetre * 3.5 / 8, hFenetre * 0.699);

    char moyen[] = "MOYEN";
    taillePolice = lFenetre / 30;
    afficheChaine(moyen, taillePolice, lFenetre * 3.5 / 8, hFenetre * 0.445);

    char difficile[] = "DIFFICILE";
    taillePolice = lFenetre / 30;
    afficheChaine(difficile, taillePolice, lFenetre * 0.425, hFenetre * 0.19);
}
