#ifndef GRILLE_H
#define GRILLE_H

typedef struct {
    char etat;
    bool flagged;
    bool hasMine;
    bool revealed;
    int nbMinesAdj;
} Case;

Case** initTableau(int lignes, int colonnes);
void placeMines(int lignes, int colonnes, int nbMines, Case** tableau, int firstx, int firsty);
void compteMines(Case** tableau, int x, int y, int lignes, int colonnes);
void afficheGrille(Case** tableau, int lignes, int colonnes, int cptflag);
void libererGrille(Case** tableau, int lignes);
void choixDifficulte(int difficulte, int* lignes, int* colonnes, int* mines, int* cptflag);
void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes, bool caseSelected);
void placerDrapeau(Case** tableau, int x, int y, int* cptflag);
void enleverDrapeau(Case** tableau, int x, int y, int* cptflag);
bool victoire(Case** tableau, int lignes, int colonnes, int nbMines);
void afficheGrilleComplete(Case** tableau, int lignes, int colonnes);

#endif // GRILLE_H
