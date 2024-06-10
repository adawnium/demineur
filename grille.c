#include <stdio.h>
#include "source/GfxLib.h"
#include <time.h>
#include <stdlib.h>


#define MINE -1

void initGrille(int lignes, int colonnes) {
    int i, j;

    int** grid = (int**)malloc(lignes * sizeof(int*));
    for (i = 0; i < lignes; i++) {
        grid[i] = (int*)malloc(colonnes * sizeof(int));
        for (j = 0; j < colonnes; j++) {
            grid[i][j] = 0;
        }
    }
}
