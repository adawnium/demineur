#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"

void choixDifficulte(int difficulte, int* lignes, int* colonnes, int* mines, int* cptflag) {
	switch (difficulte) {
		case 1:
			*lignes = 15;
			*colonnes = 10;
			*mines = 18;
			*cptflag = 18;
			break;
		case 2:
			*lignes = 20;
			*colonnes = 15;
			*mines = 45;
			*cptflag = 45;
			break;
		case 3:
			*lignes = 25;
			*colonnes = 20;
			*mines = 100;
			*cptflag = 100;
			break;
		case 4:
			*lignes = 5;
			*colonnes = 5;
			*mines = 3;
			*cptflag = 3;
			break;
		default:
			printf("Erreur de saisie\n");
			break;
	}
}

Case** initTableau(int lignes, int colonnes) {
	Case** tableau = (Case**) malloc(lignes * sizeof(Case*));
	for (int i = 0; i < lignes; i++) {
		tableau[i] = (Case*) malloc(colonnes * sizeof(Case));
		for (int j = 0; j < colonnes; j++) {
			tableau[i][j].etat = '.';
			tableau[i][j].flagged = false;
			tableau[i][j].hasMine = false;
			tableau[i][j].revealed = false;
			tableau[i][j].nbMinesAdj = 0;
		}
	}
	return tableau;
}

void afficheGrille(Case** tableau, int lignes, int colonnes, int cptflag) {
	for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
			if (!tableau[i][j].revealed) {
				if (tableau[i][j].flagged) {
					printf("F ");
				} else {
					printf(". ");
				}
			} else {
				printf("%c ", tableau[i][j].nbMinesAdj + '0');
			}
		}
		printf("\n");
	}
	printf("Nombre de drapeau(x) restant(s) : %d\n", cptflag);
}

void afficheGrilleComplete(Case** tableau, int lignes, int colonnes) {
	for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
			if (tableau[i][j].hasMine) {
				printf("M ");
			} else {
				printf("%c ", tableau[i][j].nbMinesAdj + '0');
			}
		}
		printf("\n");
	}
	printf("\n");
}

void libererGrille(Case** tableau, int lignes) {
	for (int i = 0; i < lignes; i++) {
		free(tableau[i]);
	}
	free(tableau);
}

void placeMines(int lignes, int colonnes, int mines, Case** tableau, int x1er, int y1er) {
	for(int k = 0; k < mines; k++) {
		int i, j;
		do {
			i = rand() % lignes;
			j = rand() % colonnes;
		} while(tableau[i][j].hasMine || (abs(i - x1er + 1) <= 1 && abs(j - y1er + 1) <= 1));
		tableau[i][j].hasMine = true;
	}
}

void compteMines(Case** tableau, int x, int y, int lignes, int colonnes) {
	int compteur = 0;
	if (!tableau[x][y].hasMine) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int nx = x + i;
				int ny = y + j;
				if (nx >= 0 && nx < lignes && ny >= 0 && ny < colonnes && tableau[nx][ny].hasMine) {
					compteur++;
				}
			}
		}
		tableau[x][y].nbMinesAdj = compteur;
	}
}

void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes, bool caseSelected) {
	if (x < 0 || x >= lignes || y < 0 || y >= colonnes || tableau[x][y].revealed) {
		if(caseSelected){
			printf("Choisis une autre case\n");
		}
		return;
	}
	if (tableau[x][y].flagged) {
		printf("Ouvre les knoeils y'a un drapeau la tsssss\n");
		return;
	}
	tableau[x][y].revealed = true;
	tableau[x][y].etat = tableau[x][y].nbMinesAdj + '0';
	caseSelected = false;
	if (tableau[x][y].nbMinesAdj == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				reveleCase(tableau, x + i, y + j, lignes, colonnes, caseSelected);
			}
		}
	}
}

void placerDrapeau(Case** tableau, int x, int y, int* cptflag) {
	if (tableau[x][y].revealed || tableau[x][y].flagged) {
		printf("Ouvre les knoeils y'a un drapeau la tsssss\n");
		return;
	}
	tableau[x][y].flagged = true;
	tableau[x][y].etat = 'F';
	*cptflag -= 1;
}

void enleverDrapeau(Case** tableau, int x, int y, int* cptflag) {
	if (tableau[x][y].revealed || !tableau[x][y].flagged) {
		printf("Ouvre les knoeils y'a pas de drapeau la tsssss\n");
		return;
	}
	tableau[x][y].flagged = false;
	tableau[x][y].etat = '.';
	*cptflag += 1;
}

bool victoire(Case** tableau, int lignes, int colonnes, int nbMines) {
	int cpt = 0;
	for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
			if (tableau[i][j].revealed && !tableau[i][j].hasMine) {
				cpt++;
			}
		}
	}
	return cpt == (lignes * colonnes) - nbMines;
}

int main() {
	bool rejouer = true;
	srand(time(NULL));

	while(rejouer) {
		int lignes = 0;
		int colonnes = 0;
		int mines = 0;
		int difficulte = 0;
		int x = 0;
		int y = 0;
		int choix = 0;
		int cptflag = 0;
		bool caseselected = true;
		rejouer = false;

		printf("Choisissez la difficulte :\n1. Facile\n2. Moyen\n3. Difficile\n4. Test\n");
		scanf("%d", &difficulte);

		choixDifficulte(difficulte, &lignes, &colonnes, &mines, &cptflag);

		Case** tableau = initTableau(lignes, colonnes);
		afficheGrille(tableau, lignes, colonnes, cptflag);

		printf("Entrez les coordonnees de la premiere case a devoiler (y x) : ");
		scanf("%d %d", &x, &y);

		placeMines(lignes, colonnes, mines, tableau, x, y);

		for (int i = 0; i < lignes; i++) {
			for (int j = 0; j < colonnes; j++) {
				compteMines(tableau, i, j, lignes, colonnes);
			}
		}
		reveleCase(tableau, x - 1, y - 1, lignes, colonnes, caseselected);

		while (!victoire(tableau, lignes, colonnes, mines)) {
			afficheGrille(tableau, lignes, colonnes, cptflag);
			printf("1. Reveler une case\n2. Placer un drapeau\n3. Enlever un drapeau\n");
			scanf("%d", &choix);
			switch (choix) {
				case 1:
					printf("Entrez les coordonnees de la case (x y) : ");
					scanf("%d %d", &x, &y);
					reveleCase(tableau, x - 1, y - 1, lignes, colonnes, caseselected);
					if (tableau[x - 1][y - 1].hasMine && tableau[x - 1][y - 1].revealed) {
						printf("PERDU TU ES BETEUH\n\n");
						afficheGrilleComplete(tableau, lignes, colonnes);
						return 0; // Fin du programme si perdu
					}
					break;
				case 2:
					printf("Entrez les coordonnees de la case (x y) : ");
					scanf("%d %d", &x, &y);
					placerDrapeau(tableau, x - 1, y - 1, &cptflag);
					break;
				case 3:
					printf("Entrez les coordonnees de la case (x y) : ");
					scanf("%d %d", &x, &y);
					enleverDrapeau(tableau, x - 1, y - 1, &cptflag);
					break;
				default:
					printf("Veuillez choisir un chiffre entre 1 et 3\n");
					break;
			}
		}
		if (victoire(tableau, lignes, colonnes, mines)) {
			printf("WHOWHWOHWOWHOHWOWHOHWOWHOWHOWHOHWOWHHO SPECE DE NOOOOOB T'A GAGNE\n");
			afficheGrilleComplete(tableau, lignes, colonnes);
		}
		printf("Veux tu rejouer ? (oui = 1)\n");
		scanf("%d", &choix);
		if (choix == 1) {
			rejouer = true;
		}
		libererGrille(tableau, lignes);
	}
	return 0;
}
