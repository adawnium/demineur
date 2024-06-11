#include <stdio.h>
#include "GFXLIB/GfxLib.h"
#include <time.h>
#include <stdlib.h>
#include "grille.h"

void choixDifficulte(int difficulte , int* lignes, int* colonnes, int* mines){
	switch(difficulte){
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
	}
}

int** initGrille(int lignes, int colonnes) {
    int i, j;

    int** grid = (int**)malloc(lignes * sizeof(int*));
    for (i = 0; i < lignes; i++) {
        grid[i] = (int*)malloc(colonnes * sizeof(int));
        for (j = 0; j < colonnes; j++) {
            grid[i][j] = 0;
        }
    }
   return grid;
}

void placerMines(int** grille, int lignes, int colonnes, int mines, int firstX, int firstY) {
    srand(time(NULL));
    int placed = 0;
    grille[firstX][firstY] = -1;
    for (int i = firstX - 1; i <= firstX + 1; i++) {
        for (int j = firstY - 1; j <= firstY + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                grille[i][j] = -1;
            }
        }
    }
    while (placed < mines) {
        int x = rand() % lignes;
        int y = rand() % colonnes;
        
        if ((x >= firstX - 1 && x <= firstX + 1) && (y >= firstY - 1 && y <= firstY + 1)) {
            continue;
        }

        if (grille[x][y] == 0) {
            grille[x][y] = -1;
            placed++;
        }
    }
    grille[firstX][firstY] = 0;
    for (int i = firstX - 1; i <= firstX + 1; i++) {
        for (int j = firstY - 1; j <= firstY + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                grille[i][j] = 0;
            }
        }
    }
}

void placerChiffres(int** grille, int lignes, int colonnes) {
    int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (grille[i][j] == -1) continue;
            int minesCount = 0;
            for (int k = 0; k < 8; k++) {
                int ni = i + dx[k];
                int nj = j + dy[k];
                if (ni >= 0 && ni < lignes && nj >= 0 && nj < colonnes && grille[ni][nj] == -1) {
                    minesCount++;
                }
            }
            grille[i][j] = minesCount;
        }
    }
}

void libererGrille(int** grille, int lignes) {
    for (int i = 0; i < lignes; i++) {
        free(grille[i]);
    }
    free(grille);
}

void afficherGrille(int** grille, int lignes, int colonnes) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (grille[i][j] == -1) {
                printf("* ");
            } else {
                printf("%d ", grille[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int lignes, colonnes, mines;
    choixDifficulte(1, &lignes, &colonnes, &mines);

    int** grille = initGrille(lignes, colonnes);
    
    int firstX = 0;
    int firstY = 0;
    
    placerMines(grille, lignes, colonnes, mines, firstX, firstY);
    placerChiffres(grille, lignes, colonnes);

    afficherGrille(grille, lignes, colonnes);

    libererGrille(grille, lignes);
    return 0;
}
