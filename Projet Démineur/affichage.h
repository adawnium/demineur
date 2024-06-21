#ifndef AFFICHAGE_H
#define AFFICHAGE_H

void pageJeu(int lignes, int colonnes, bool fondBlanc, bool win, bool minetouche, Case **tableau, int cptflag, time_t startTime, double elapsedTime, bool firstclick, bool poweruse, bool relancement);
void pageAccueil(bool fondBlanc);

#endif // AFFICHAGE_H
