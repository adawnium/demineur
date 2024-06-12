#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"

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
bool victoire(Case** tableau, int lignes, int colonnes);
void afficheGrilleDefaite(Case** tableau, int lignes, int colonnes);
bool finDePartie();
//??????????????????????????????????????????????????????????????????????????


//TODO: fonctions qui check les victoires
//TODO: fonctions qui vérifie que toutes les cases ont été révélées
//TODO: fonction fin de partie



void choixDifficulte(int difficulte , int* lignes, int* colonnes, int* mines) {
	switch(difficulte) {
		case 1:
			*lignes = 15;
			*colonnes = 10;
			*mines = 18;
			break;
		case 2:
			*lignes = 20;
			*colonnes = 15;
			*mines = 45;
			break;
		case 3:
			*lignes = 25;
			*colonnes = 20;
			*mines = 100;
			break;
        case 4: //cas de test
            *lignes = 5;
            *colonnes = 5;
            *mines = 2;
            break;
        default:
            printf("Erreur de saisie\n");
            break;
	}
}


//Initialise le tableau 2D de cases
Case** initTableau(int lignes, int colonnes) {
    Case** tableau = (Case**) malloc(lignes * sizeof(Case*));
    for (int i = 0; i < lignes; i++) {
        tableau[i] = (Case*) malloc(colonnes * sizeof(Case));
        for (int j = 0; j < colonnes; j++) {
            tableau[i][j].etat = '.';
            tableau[i][j].flagged = false;
            tableau[i][j].hasMine = false;
            tableau[i][j].revealed = false;  // TODO: changer à false
            tableau[i][j].nbMinesAdj = 0;
        }
    }
    return tableau;
}

void afficheGrille(Case** tableau, int lignes, int colonnes) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if(tableau[i][j].flagged) {
                printf("F ");
            } 
            // else if(!tableau[i][j].revealed) {
            //     printf(". ");
            // }
            else if(tableau[i][j].revealed) {
                if(tableau[i][j].hasMine) {
                    printf("M ");
                } else {
                    printf("%c ", tableau[i][j].nbMinesAdj + '0');
                }
            }
            else {
                printf("%c ", tableau[i][j].etat);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficheGrilleDefaite(Case** tableau, int lignes, int colonnes) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if(tableau[i][j].hasMine) {
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

//Place les mines aléatoirement dans le tableau
void placeMines(int lignes, int colonnes, int nbMines, Case** tableau, int firstx, int firsty) {
    // srand(time(NULL));
    // int cptMines = 0;

    // while (cptMines < nbMines) {
    //     int x = rand() % lignes;
    //     int y = rand() % colonnes;
    //     if (!tableau[x][y].hasMine) {
    //         tableau[x][y].hasMine = true;
    //         tableau[x][y].etat = 'M';
    //         cptMines++;
    //     }
    // }
    
    srand(time(NULL));
    int cptMines = 0;

    tableau[firstx][firsty].hasMine = true;
    tableau[firstx][firsty].etat = 'M';
    for (int i = firstx - 1; i <= firstx + 1; i++) {
        for (int j = firsty - 1; j <= firsty + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                tableau[i][j].hasMine = true;
                tableau[i][j].etat = 'M';
            }
        }
    }

    while (cptMines < nbMines) {
        int x = rand() % lignes;
        int y = rand() % colonnes;
        if (!tableau[x][y].hasMine) {
            tableau[x][y].hasMine = true;
            tableau[x][y].etat = '.';
            cptMines++;
        }
    }

    tableau[firstx][firsty].hasMine = false;
    tableau[firstx][firsty].etat = '.';
    for (int i = firstx - 1; i <= firstx + 1; i++) {
        for (int j = firsty - 1; j <= firsty + 1; j++) {
            if (i >= 0 && i < lignes && j >= 0 && j < colonnes) {
                tableau[i][j].hasMine = false;
                tableau[i][j].etat = '.';
            }
        }
    }
}

//Compte le nombre de mines adjacentes à une case
void compteMines(Case** tableau, int x, int y, int lignes, int colonnes) {
    int compteur = 0;

    if(!tableau[x][y].hasMine) {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(x + i >= 0 && x + i < lignes && y + j >= 0 && y + j < colonnes) {
                    if(tableau[x + i][y + j].hasMine) {
                        compteur++;
                    }
                }
            }
        }
        tableau[x][y].nbMinesAdj = compteur;
    }
}

//Révèle la case et les cases adjacentes 
void reveleCase(Case** tableau, int x, int y, int lignes, int colonnes) {
    if(x < 0 || x >= lignes || y < 0 || y >= colonnes) {
        return;
    }
    if(tableau[x][y].revealed) {
        return;
    }

    tableau[x][y].revealed = true;
    tableau[x][y].etat = tableau[x][y].nbMinesAdj + '0';

    if(tableau[x][y].nbMinesAdj == 0) {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                reveleCase(tableau, x + i, y + j, lignes, colonnes);
            }
        }
    }
}

//Place un drapeau sur la case
void placerDrapeau(Case** tableau, int x, int y) {
    if(tableau[x][y].revealed || tableau[x][y].flagged) {
        return;
    }

    tableau[x][y].flagged = true;
    tableau[x][y].etat = 'F';
}

//Enlève le drapeau de la case
void enleverDrapeau(Case** tableau, int x, int y) {
    if(tableau[x][y].revealed || !tableau[x][y].flagged) {
        return;
    }

    tableau[x][y].flagged = false;
    tableau[x][y].etat = '.';
}


///////////////////////A REVOIR///////////////////////////
int verifCasesRevelees(Case** tableau, int lignes, int colonnes) {
    for(int i = 0; i < lignes; i++) {
        for(int j = 0; j < colonnes; j++) {
            if(!tableau[i][j].revealed && !tableau[i][j].hasMine) {
                return 0;
            }
        }
    }
    return 1;
}

bool victoire(Case** tableau, int lignes, int colonnes) {
    for(int i = 0; i < lignes; i++) {
        for(int j = 0; j < colonnes; j++) {
            if(tableau[i][j].flagged && tableau[i][j].hasMine) {
                return true;
            }
        }
    }
    return false;
}

// bool finDePartie() {}

// int allCasesRevelees(Case** tableau, int lignes, int colonnes) {
//     int cpt = 0;
//     for(int i = 0; i < lignes; i++) {
//         for(int j = 0; j < colonnes; j++) {
//             if(tableau[i][j].revealed && !tableau[i][j].hasMine){
//                 cpt++;
//             }
//         }
//     }   
// }

void Chronometre(clock_t start, clock_t end, double cpu_time_used) {
    if (initTableau != NULL) {
        for (int i = 0; i < 1000000; i++) {
            start = clock();
            cpu_time_used = ((double) (end - start)) / CLOCK_PER_SEC;
        }
    }
}

///////////////////////////////////////////////////////////

int main() {
    int lignes = 0;
    int colonnes = 0;
    int mines = 0;
    int difficulte = 0;
    int x1er = 0;
    int y1er = 0;


    printf("Choisissez la difficulte :\n1. Facile\n2. Moyen\n3. Difficile\n4. Test\n");
    scanf("%d", &difficulte);

    choixDifficulte(difficulte, &lignes, &colonnes, &mines);

    Case** tableau = initTableau(lignes, colonnes);
    afficheGrille(tableau, lignes, colonnes);

    printf("Entrez les coordonnees de la premiere case a devoiler (x y) : ");
    scanf("%d %d", &x1er, &y1er);

    placeMines(lignes, colonnes, mines, tableau, x1er, y1er);

    for(int i = 0; i < lignes; i++) {
        for(int j = 0; j < colonnes; j++) {
            compteMines(tableau, i, j, lignes, colonnes);
        }
    }

    int gameTermine = verifCasesRevelees(tableau, lignes, colonnes); 
    // bool aGagne = victoire(tableau, lignes, colonnes);

    while(gameTermine == 0) {
        afficheGrille(tableau, lignes, colonnes);
        int x = 0;
        int y = 0;
        int choix = 0;


        printf("1. Reveler une case\n2. Placer un drapeau\n3. Enlever un drapeau\n");
        scanf("%d", &choix);

        printf("Entrez les coordonnees de la case (x y) : ");
        scanf("%d %d", &x, &y);

        if(choix == 1) {
            reveleCase(tableau, x - 1, y - 1, lignes, colonnes);
            if(tableau[x-1][y-1].hasMine) {
                printf("PERDU TU ES BETEUH\n\n");
                afficheGrilleDefaite(tableau, lignes, colonnes);
                break;
            }
        } else if(choix == 2) {
            placerDrapeau(tableau, x - 1, y - 1);
        } else if(choix == 3) {
            enleverDrapeau(tableau, x - 1, y - 1);
        }

        gameTermine = verifCasesRevelees(tableau, lignes, colonnes);
        // aGagne = victoire(tableau, lignes, colonnes);
    }


    libererGrille(tableau, lignes);


    return 0;
}