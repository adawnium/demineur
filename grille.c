#include <stdio.h>
#include "GfxLib.h"
#include <time.h>
#include "grille.h"

#define MINE -1

void initGrille(int lignes, int colonnes) {
    int i = 0, j = 0;

    int** grid = (int**)malloc(lignes * sizeof(int*));
    for (i = 0; i < lignes; i++) {
        grid[i] = (int*)malloc(colonnes * sizeof(int));
        for (j = 0; j < colonnes; j++) {
            grid[i][j] = 0;
        }
    }
}

int main() {
    return 0;
}
