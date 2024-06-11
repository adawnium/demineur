#ifndef GRILLE_H

#define GRILLE_H

int **initGrille(int lignes, int colonnes);

void choixDifficulte(int difficulte, int *lignes, int *colonnes, int *mines);

void placerMines(int** grille, int lignes, int colonnes, int mines, int firstX, int firstY);

void placerChiffres(int **grille, int lignes, int colonnes);

void libererGrille(int **grille, int lignes);

void afficherGrille(int **grille, int lignes, int colonnes);

#endif // GRILLE_H