#ifndef GRILLE_H
#define GRILLE_H

//La structure des cases du tableau
typedef struct {
    char etat;
    bool flagged;
    bool hasMine;
    bool revealed;
    int nbMinesAdj;
} Case;

//Prototypes des fonctions
Case** initTableau(int lignes, int colonnes);
void placeMines(int lignes, int colonnes, int nbMines, Case** tableau, int firstx, int firsty);
void compteMines(Case** tableau, int x, int y, int lignes, int colonnes);
void afficheGrille(Case** tableau, int lignes, int colonnes);
void libererGrille(Case** tableau, int lignes);
void choixDifficulte(int difficulte, int* lignes, int* colonnes, int* mines);
void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes);
void placerDrapeau(Case** tableau, int x, int y);
void enleverDrapeau(Case** tableau, int x, int y);
int verifCasesRevelees(Case** tableau, int lignes, int colonnes);
bool victoire(Case** tableau, int lignes, int colonnes, int nbMines);
void afficheGrilleComplete(Case** tableau, int lignes, int colonnes);

#endif // GRILLE_H
