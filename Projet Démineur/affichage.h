#ifndef AFFICHAGE_H
#define AFFICHAGE_H

void gestionEvenement(EvenementGfx evenement);
void dessineGrille(int lignes, int colonnes, bool fondBlanc, bool win, bool minetouche, Case **tableau, int cptflag);
void pageAccueil(bool fondBlanc);

#endif // AFFICHAGE_H
