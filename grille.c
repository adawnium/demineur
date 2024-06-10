#include <stdio.h>
#include "GfxLib.h"

void initGrille(int lignes, int colonnes, int mines) {
    int i, j, cpt;

    int** grid = (int**)malloc(lignes * sizeof(int*));
    for (i = 0; i < lignes; i++) {
        grid[i] = (int*)malloc(colonnes * sizeof(int));
        for (j = 0; j < colonnes; j++) {
            grid[i][j] = 0;
        }
    }

    srand(time(0));
    while (cpt < mines) {
        int x = rand() % lignes;
        int y = rand() % colonnes;
        if (grid[x][y] != MINE) {
            grid[x][y] = MINE;
            cpt++;
        }
    }
}
