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
#define LargeurFenetre 800  // Augmenté pour inclure l'espace du score
#define HauteurFenetre 600
#define LARGEUR_SCORE 400

void gestionEvenement(EvenementGfx evenement) {
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
    static time_t endTime = 0;
    static double elapsedTime =0;
    

    switch (evenement) {
        case Initialisation:
            demandeTemporisation(20);
            break;
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:
            if (fondBlanc) {
                effaceFenetre(255, 255, 255);
            } else {
                effaceFenetre(0, 0, 0);
            }
            if (difficulte == 0) {
                pageAccueil(fondBlanc);
            } else {
                dessineGrille(lignes, colonnes, fondBlanc, win, minetouche, tableau, cptflag, startTime, elapsedTime, firstclick);
            }
            break;
        case Clavier:
            switch (caractereClavier()) {
                case 'Q':
                case 'q':
                    termineBoucleEvenements();
                    break;
                case 'F':
                case 'f':
                    pleinEcran = !pleinEcran;
                    if (pleinEcran) {
                        modePleinEcran();
                    } else {
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
            }
            break;
        case ClavierSpecial:
            break;
        case BoutonSouris:
            switch (etatBoutonSouris()) {
                case GaucheAppuye:
                	int x = abscisseSouris();
                        int y = ordonneeSouris();
                    if ((win || minetouche) && x >= largeurFenetre()*1.5/8 && x <= largeurFenetre()*3.5/8 && y >= hauteurFenetre()*4/12 && y <= hauteurFenetre()*6/12) {
                        // Rejouer
                        firstclick = false;
                        minetouche = false;
                        win = false;
                        libererGrille(tableau, lignes);
                        tableau = initTableau(lignes, colonnes);
                        afficheGrille(tableau, lignes, colonnes, cptflag);
                    } else if ((win || minetouche) && x >= largeurFenetre()*4.5/8 && x <= largeurFenetre()*6.5/8 && y >= hauteurFenetre()*4/12 && y <= hauteurFenetre()*6/12) {
                        // Quitter
                        termineBoucleEvenements();
                    }
                    if (!minetouche && !win) {
                        gererClicGauche(x, y, &lignes, &colonnes, &difficulte, &mines, &cptflag, &dehors, &firstclick, tableau, &minetouche, &win, &initialise, &startTime);
                        if (difficulte != 0 && !initialise) {
                            tableau = initTableau(lignes, colonnes);
                            initialise = !initialise;
                        }
                        if (initialise && !dehors && !minetouche) {
                            afficheGrille(tableau, lignes, colonnes, cptflag);
                        }
                        if (minetouche) {
                            printf("PERDU TU ES BETEUH\n\n");
                            afficheGrilleComplete(tableau, lignes, colonnes);
                        }
                        if (win) {
                            printf("GG TU AS GAGNE\n\n");
                            afficheGrilleComplete(tableau, lignes, colonnes);   
                        }
                    }
                    break;
                case GaucheRelache:
                    break;
                case DroiteAppuye:
                    if (!minetouche && firstclick) {
                        int x = abscisseSouris();
                        int y = ordonneeSouris();
                        gererClicDroit(x, y, lignes, colonnes, difficulte, tableau, &cptflag);
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
        case Souris:
            break;
        case Inactivite:
            break;
        case Redimensionnement:
            printf("Largeur : %d\t", largeurFenetre());
            printf("Hauteur : %d\n", hauteurFenetre());
            break;
    }
}
void dessineGrille(int lignes, int colonnes, bool fondBlanc, bool win, bool minetouche, Case **tableau, int cptflag, time_t startTime, double elapsedTime, bool firstclick) {
	int largeur = largeurFenetre();
	int largeurgrille = largeurFenetre() - LARGEUR_SCORE;
	int hauteur = hauteurFenetre();
	int largeurCase = largeurgrille / colonnes;
	int hauteurCase = hauteur / lignes;
	int tailleCase = largeurCase < hauteurCase ? largeurCase : hauteurCase;
	if(fondBlanc)
	{
	       couleurCourante(0, 0, 0);
	}
	else{
	       couleurCourante(255, 255, 255);
	}

	    // Dessiner les cases de la grille
	    for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
		    int x = j * tailleCase + 1;
		    int y = i * tailleCase + 1;
		    rectangle(x, y, x + tailleCase - 1, y + tailleCase - 1);
		}
	    }
    	epaisseurDeTrait(1);
        char titre[] = "DEMINEUR";
        int taillePolice = 25;
        afficheChaine(titre, taillePolice, largeur*5/8, hauteur*11/12);
        
        
        char chrono[20];
        if(firstclick && !win && !minetouche)
        { 
		time_t currentTime;
		time(&currentTime);
		elapsedTime = difftime(currentTime, startTime);
	}
	sprintf(chrono, "Chronometre: %.0f s", elapsedTime);
        afficheChaine(chrono, taillePolice, largeur*5/8, hauteur*9.5/12);
        
        
        char flag[50];
        sprintf(flag, "Mines restantes: %d", cptflag);
        afficheChaine(flag, taillePolice, largeur*5/8, hauteur*8/12);
        
        rectangle(largeur*5/8, hauteur*7/12, largeur*7/8, hauteur*5.5/12);
        rectangle(largeur*5/8, hauteur*4.5/12, largeur*7/8, hauteur*3/12);
        rectangle(largeur*5/8, hauteur*2/12, largeur*7/8, hauteur*0.5/12);
        
        if(fondBlanc)
        {
      		couleurCourante(255, 255, 255);
      	}
      	else{
      		couleurCourante(0, 0, 0);
      	}
        
        taillePolice = 20;
        char recommencer[] = "Recommencer";
        afficheChaine(recommencer, taillePolice, largeur*5.3/8, hauteur*6.25/12);
        char EcranTitre[] = "Ecran titre";
        afficheChaine(EcranTitre, taillePolice, largeur*5.5/8, hauteur*1.25/12);
        taillePolice = 15;
        char switchNiveau[] = "Changer de niveau";
        afficheChaine(switchNiveau, taillePolice, largeur*5.25/8, hauteur*3.75/12);
        if(win || minetouche){
        	if(fondBlanc)
		{
	      		couleurCourante(255, 255, 255);
	      	}
	      	else{
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
		    couleurCourante(255, 0, 0); // Rouge pour les lignes surlignées

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

		    epaisseurDeTrait(1);// Peut-être rajouter des lignes pour que ça se voit plus
        	if(fondBlanc)
		{
	      		couleurCourante(0, 0, 0);
	      	}
	      	else{
	      		couleurCourante(255, 255, 255);
	      	}
        	rectangle(largeur*1.5/8, hauteur*6/12, largeur*3.5/8, hauteur*4/12);
        	rectangle(largeur*4.5/8, hauteur*6/12, largeur*6.5/8, hauteur*4/12);
        	taillePolice=30;
        	if(win){
        		char gagne[]="GG TU AS GAGNE";
        		afficheChaine(gagne, taillePolice, largeur*2.5/8, hauteur*7.5/12);
        	}
        	if(minetouche){
        		char perdu[]="PERDU TU ES BETEUH";
        		afficheChaine(perdu, taillePolice, largeur*2/8, hauteur*7.5/12);
        	}
        	if(fondBlanc)
		{
	      		couleurCourante(255, 255, 255);
	      	}
	      	else{
	      		couleurCourante(0, 0, 0);
	      	}
        	taillePolice=15;
        	char rejouer[]="Rejouer";
        	afficheChaine(rejouer, taillePolice, largeur*2.25/8, hauteur*5/12);
        	char quitter[]="Quitter";
        	afficheChaine(quitter, taillePolice, largeur*5.25/8, hauteur*5/12);
        	
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
