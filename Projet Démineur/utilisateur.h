#ifndef UTILISATEUR_H
#define UTILISATEUR_H

void gererClicGauche(int x, int y, int *lignes, int *colonnes, int *difficulte, int *mines, int *cptflag, bool *dehors, bool *firstclick, Case **tableau, bool *minetouche, bool *win, bool *initialise, time_t *startTime, bool *powerstock, bool *poweruse, bool *relancement);
void gererClicDroit(int x, int y, int lignes, int colonnes, int difficulte, Case** tableau, int *cptflag);

#endif // UTILISATEUR_H
