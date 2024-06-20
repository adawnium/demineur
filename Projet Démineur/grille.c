#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"

void choixDifficulte(int difficulte, int* lignes, int* colonnes, int* mines, int* cptflag) {
	switch (difficulte) {
		case 1:
			*lignes = 15;
			*colonnes = 10;
			*mines = 18;
			*cptflag = 18;
			break;
		case 2:
			*lignes = 20;
			*colonnes = 15;
			*mines = 45;
			*cptflag = 45;
			break;
		case 3:
			*lignes = 25;
			*colonnes = 20;
			*mines = 100;
			*cptflag = 100;
			break;
		case 4:
			*lignes = 5;
			*colonnes = 5;
			*mines = 3;
			*cptflag = 3;
			break;
		default:
			break;
	}
}

// Initialise le tableau 2D de cases
Case** initTableau(int lignes, int colonnes) {
	Case** tableau = (Case**) malloc(lignes * sizeof(Case*));
	for (int i = 0; i < lignes; i++) {
		tableau[i] = (Case*) malloc(colonnes * sizeof(Case));
		for (int j = 0; j < colonnes; j++) {
			tableau[i][j].etat = '.';
			tableau[i][j].flagged = false;
			tableau[i][j].hasMine = false;
			tableau[i][j].revealed = false;
			tableau[i][j].nbMinesAdj = 0;
		}
	}
	return tableau;
}

void afficheGrille(Case** tableau, int lignes, int colonnes, int cptflag) {
    if (tableau == NULL) {
        printf("Pointeur nul (afficheGrille)\n");
        return;
    }
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (!tableau[i][j].revealed) {
                if (tableau[i][j].flagged) {
                    printf("F ");
                } else {
                    printf(". ");
                }
            } else {
                printf("%c ", tableau[i][j].nbMinesAdj + '0');
            }
        }
        printf("\n");
    }
    printf("Nombre de drapeau restant : %d\n", cptflag);
}

void afficheGrilleComplete(Case** tableau, int lignes, int colonnes) {
    if (tableau == NULL) {
        printf("Pointeur nul (afficheGrilleComplete)\n");
        return;
    }
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (tableau[i][j].hasMine) {
                printf("M ");
            } else {
                printf("%c ", tableau[i][j].nbMinesAdj + '0');
            }
        }
        printf("\n");
    }
    printf("\n");
}

void libererGrille(Case** tableau, int lignes) {
    if (tableau == NULL) {
        printf("Pointeur nul (libererGrille)\n");
        return;
    }
    for (int i = 0; i < lignes; i++) {
        free(tableau[i]);
    }
    free(tableau);
}

void placeMines(int lignes, int colonnes, int mines, Case** tableau, int x, int y) {
    if (tableau == NULL) {
        printf("Pointeur nul (placeMines)\n");
        return;
    }
    srand(time(NULL));
    for(int k = 0; k < mines; k++) {
        int i, j;
        do {
            i = rand() % lignes;
            j = rand() % colonnes;
        } while(tableau[i][j].hasMine || (abs(i - x) <= 1 && abs(j - y) <= 1)); // Continue tant qu'une mine est déjà présente ou que la case est voisine de la première case à dévoiler
        tableau[i][j].hasMine = true;
    }
}

void compteMines(Case** tableau, int x, int y, int lignes, int colonnes) {
    if (tableau == NULL) {
        printf("Pointeur nul (compteMines)\n");
        return;
    }
    int compteur = 0;
    if (!tableau[x][y].hasMine) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && nx < lignes && ny >= 0 && ny < colonnes && tableau[nx][ny].hasMine) {
                    compteur++;
                }
            }
        }
        tableau[x][y].nbMinesAdj = compteur;
    }
}

void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes, bool caseselected) {
    if (tableau == NULL) {
        printf("Pointeur nul (reveleCase)\n");
        return;
    }
    if (x < 0 || x >= lignes || y < 0 || y >= colonnes || tableau[x][y].revealed) {
        if(caseselected){
            printf("Choisis une autre case\n");
        }
        return;
    }
    if (tableau[x][y].flagged) {
        printf("Ouvre les knoeils y'a un drapeau la tsssss\n");
        return;
    }
    
    tableau[x][y].revealed = true;
    if(!tableau[x][y].hasMine){
    tableau[x][y].etat = tableau[x][y].nbMinesAdj + '0';
    }
    else{
    	tableau[x][y].etat = 'M';
    }
    caseselected = false;
    if (tableau[x][y].nbMinesAdj == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                reveleCase(tableau, x + i, y + j, lignes, colonnes, caseselected);
            }
        }
    }
}

void placerDrapeau(Case** tableau, int x, int y, int* cptflag) {
    if (tableau == NULL) {
        printf("Pointeur nul (placerDrapeau)\n");
        return;
    }
    if (tableau[x][y].revealed || tableau[x][y].flagged) {
        printf("Ouvre les knoeils y'a un drapeau la tsssss\n");
        return;
    }
    tableau[x][y].flagged = true;
    tableau[x][y].etat = 'F';
    *cptflag -= 1;
}

void enleverDrapeau(Case** tableau, int x, int y, int* cptflag) {
    if (tableau == NULL) {
        printf("Pointeur nul (enleverDrapeau)\n");
        return;
    }
    if (tableau[x][y].revealed || !tableau[x][y].flagged) {
        printf("Ouvre les knoeils y'a pas de drapeau la tsssss\n");
        return;
    }
    tableau[x][y].flagged = false;
    tableau[x][y].etat = '.';
    *cptflag += 1;
}

bool victoire(Case** tableau, int lignes, int colonnes, int nbMines) {
    if (tableau == NULL) {
        printf("Pointeur nul (victoire)\n");
        return false;
    }
    int cpt = 0;
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (tableau[i][j].revealed && !tableau[i][j].hasMine) {
                cpt++;
            }
        }
    }
    return cpt == (lignes * colonnes) - nbMines;
}
