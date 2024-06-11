#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "source/GfxLib.h"

//La structure des cases du tableau
typedef struct {
    char etat;
    bool flaged;
    bool hasMine;
    bool revealed;
    int nbMinesAdj;
} Case;

//Prototypes des fonctions
Case** initTableau(int lignes, int colonnes);
void placeMines(int lignes, int colonnes, int nbMines, Case** tableau, int x, int y);
void compteMines(Case** tableau, int x, int y, int lignes, int colonnes);
void afficheGrille(Case** tableau, int lignes, int colonnes);


void choixDifficulte(int difficulte , int* lignes, int* colonnes, int* mines) {
	switch(difficulte) {
		case 1:
			*lignes = 15;
			*colonnes = 10;
			*mines = 18;
			break;
		case 2:
			*lignes = 20;
			*colonnes = 15;
			*mines = 45;
			break;
		case 3:
			*lignes = 25;
			*colonnes = 20;
			*mines = 100;
			break;
        default:
            printf("Erreur de saisie\n");
            break;
	}
}


//Initialise le tableau 2D de cases
Case** initTableau(int lignes, int colonnes) {
    Case** tableau = (Case**) malloc(lignes * sizeof(Case*));
    for (int i = 0; i < lignes; i++) {
        tableau[i] = (Case*) malloc(colonnes * sizeof(Case));
        for (int j = 0; j < colonnes; j++) {
            tableau[i][j].etat = '.';
            tableau[i][j].flaged = false;
            tableau[i][j].hasMine = false;
            tableau[i][j].revealed = false;
            tableau[i][j].nbMinesAdj = 0;
        }
    }
    return tableau;
}

void afficheGrille(Case** tableau, int lignes, int colonnes) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            printf("%c ", tableau[i][j].etat);
        }
        printf("\n");
    }
    printf("\n");
}

void libererGrille(Case** tableau, int lignes) {
    for (int i = 0; i < lignes; i++) {
        free(tableau[i]);
    }
    free(tableau);
}

//Place les mines aléatoirement dans le tableau
void placeMines(int lignes, int colonnes, int nbMines, Case** tableau, int firstx, int firsty) {
    srand(time(NULL));
    int cptMines = 0;

    tableau[firstx][firsty].hasMine = true;
    tableau[firstx][firsty].etat = 'M';
    for (int i = firstx - 1; i <= firstx + 1; i++) {
        for (int j = firsty - 1; j <= firsty + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                tableau[i][j].hasMine = true;
                tableau[i][j].etat = 'M';
            }
        }
    }

    while (cptMines < nbMines) {
        int x = rand() % lignes;
        int y = rand() % colonnes;
        if (!tableau[x][y].hasMine) {
            tableau[x][y].hasMine = true;
            tableau[x][y].etat = 'M';
            cptMines++;
        }
    }

    tableau[firstx][firsty].hasMine = false;
    tableau[firstx][firsty].etat = '.';
    for (int i = firstx - 1; i <= firstx + 1; i++) {
        for (int j = firsty - 1; j <= firsty + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                tableau[i][j].hasMine = false;
                tableau[i][j].etat = '.';
            }
        }
    }
}

void compteMines(Case** tableau, int x, int y, int lignes, int colonnes) {
    int compteur = 0;

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(x + i >= 0 && x + i < lignes && y + j >= 0 && y + j < colonnes) {
                if(tableau[x + i][y + j].hasMine) {
                    compteur++;
                }
            }
        }
    }
    tableau[x][y].nbMinesAdj = compteur;
}


void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes) {
    if(x < 0 || x >= lignes || y < 0 || y >= colonnes || tableau[x][y].revealed) {
        return;
    }
    
    tableau[x][y].revealed = true;

    if(tableau[x][y].nbMinesAdj == 0) {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                reveleCase(tableau, x + i, y + j, lignes, colonnes);
            }
        }
    }
}




int main() {
    int lignes = 0, colonnes = 0, mines = 0 /*difficulte = 0*/;

    choixDifficulte(1, &lignes, &colonnes, &mines);

    Case** tableau = initTableau(lignes, colonnes);
    afficheGrille(tableau, lignes, colonnes);

    return 0;
}