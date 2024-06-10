#include <stdio.h>
#include "source/GfxLib.h"
#include <time.h>
#include <stdlib.h>
#include "grille.h"

void initGrille(int lignes, int colonnes) {

    int** grid = (int**)malloc(lignes * sizeof(int*));
    for (int i = 0; i < lignes; i++) {
        grid[i] = (int*)malloc(colonnes * sizeof(int));
        for (int j = 0; j < colonnes; j++) {
            grid[i][j] = 0;
        }
    }
}

void afficheGrille(int** tab, int lignes, int colonnes) {
    for(int i = 0; i < lignes; i++) {
        for(int j = 0; j < colonnes; j++) {
            printf("[%d]", tab[i][j]);
        }
        printf("\n");
    }
}



int main(void) {
    return 0;
}