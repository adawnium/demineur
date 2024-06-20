#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GFXLIB/GfxLib.h"
#include "grille.h"

// Définit les dimensions et le nombre de mines en fonction du niveau de difficulté
void choixDifficulte(int difficulte, int *lignes, int *colonnes, int *mines, int *cptflag)
{
    switch (difficulte)
    {
    case 1: // mode facile
        *lignes = 15;
        *colonnes = 10;
        *mines = 18;
        *cptflag = 18;
        break;
    case 2: //mode moyen
        *lignes = 20;
        *colonnes = 15;
        *mines = 45;
        *cptflag = 45;
        break;
    case 3: //mode difficile
        *lignes = 25;
        *colonnes = 20;
        *mines = 100;
        *cptflag = 100;
        break;
    default:
        break;
    }
}

// Initialise le tableau 2D de cases
Case **initTableau(int lignes, int colonnes)
{
    Case **tableau = (Case **)malloc(lignes * sizeof(Case *)); //allocation de mémoire 
    for (int i = 0; i < lignes; i++)
    {
        tableau[i] = (Case *)malloc(colonnes * sizeof(Case)); 
        for (int j = 0; j < colonnes; j++) 
        {
            tableau[i][j].etat = '.';
            tableau[i][j].flagged = false;
            tableau[i][j].hasMine = false;
            tableau[i][j].revealed = false; 
            tableau[i][j].nbMinesAdj = 0;
        }
    }
    return tableau;
}

// Affiche la grille en cours de jeu
void afficheGrille(Case **tableau, int lignes, int colonnes, int cptflag)
{
    if (tableau == NULL)                                // vérification de la validité du pointeur
    {
        printf("Pointeur nul (afficheGrille)\n");
        return;
    }
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (!tableau[i][j].revealed)
            {
                if (tableau[i][j].flagged)
                {                                     // condition pour que l'affichage soit cohérent
                    printf("F ");
                }
                else
                {
                    printf(". ");
                }
            }
            else
            {
                printf("%c ", tableau[i][j].nbMinesAdj + '0');
            }
        }
        printf("\n");
    }
    printf("Nombre de drapeau restant : %d\n", cptflag);
}

// Affiche la grille complète avec toutes les mines dévoilées (pour debug ou fin de partie)
void afficheGrilleComplete(Case **tableau, int lignes, int colonnes)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (afficheGrilleComplete)\n");
        return;
    }
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (tableau[i][j].hasMine)
            {
                printf("M ");
            }
            else
            {
                printf("%c ", tableau[i][j].nbMinesAdj + '0');
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Libère la mémoire allouée pour la grille
void libererGrille(Case **tableau, int lignes)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (libererGrille)\n");
        return;
    }
    for (int i = 0; i < lignes; i++)            
    {
        free(tableau[i]);   //libération de la mémoire très important surtout si on veut faire durer le programme
    }
    free(tableau);
}

// Place les mines aléatoirement dans la grille, en évitant la première case cliquée et ses voisines
void placeMines(int lignes, int colonnes, int mines, Case **tableau, int x, int y)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (placeMines)\n");
        return;
    }
    srand(time(NULL));
    for (int k = 0; k < mines; k++)
    {
        int i, j;
        do
        {
            i = rand() % lignes;
            j = rand() % colonnes;
        } while (tableau[i][j].hasMine || (abs(i - x) <= 1 && abs(j - y) <= 1)); // Evite les cases adjacentes à la première
        tableau[i][j].hasMine = true;
    }
}

// Compte les mines adjacentes à une case donnée
void compteMines(Case **tableau, int x, int y, int lignes, int colonnes)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (compteMines)\n");
        return;
    }
    int compteur = 0;
    if (!tableau[x][y].hasMine)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && nx < lignes && ny >= 0 && ny < colonnes && tableau[nx][ny].hasMine)  // compte le nombre de mine autour d'une case
                {
                    compteur++;
                }
            }
        }
        tableau[x][y].nbMinesAdj = compteur;
    }
}

// Révèle une case et, si elle est vide, révèle récursivement ses voisines
void reveleCase(Case **tableau, int x, int y, int lignes, int colonnes, bool caseselected)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (reveleCase)\n");
        return;
    }
    if (x < 0 || x >= lignes || y < 0 || y >= colonnes || tableau[x][y].revealed)  // si la case en paramètre n'est pas bonne
    {
        if (caseselected)
        {
            printf("Choisis une autre case\n");
        }
        return;
    }
    if (tableau[x][y].flagged)
    {
        printf("impossible il y a un drapeau ici\n");
        return;
    }

    tableau[x][y].revealed = true;
    if (!tableau[x][y].hasMine)
    {
        tableau[x][y].etat = tableau[x][y].nbMinesAdj + '0';
    }
    else
    {
        tableau[x][y].etat = 'M';
    }
    caseselected = false;
    if (tableau[x][y].nbMinesAdj == 0)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                reveleCase(tableau, x + i, y + j, lignes, colonnes, caseselected); // vérifie les cases autour pour voir si il y a des 0
            }
        }
    }
}

// Place un drapeau sur une case pour marquer une mine
void placerDrapeau(Case **tableau, int x, int y, int *cptflag)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (placerDrapeau)\n");
        return;
    }
    if (tableau[x][y].revealed || tableau[x][y].flagged)
    {
        printf("On ne peut pas placer un drapeau ici\n");
        return;
    }
    tableau[x][y].flagged = true;
    tableau[x][y].etat = 'F';
    *cptflag -= 1;
}

// Enlève un drapeau d'une case
void enleverDrapeau(Case **tableau, int x, int y, int *cptflag)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (enleverDrapeau)\n");
        return;
    }
    if (tableau[x][y].revealed || !tableau[x][y].flagged)
    {
        printf("Il n'y a pas de drapeau ici\n");
        return;
    }
    tableau[x][y].flagged = false;
    tableau[x][y].etat = '.';
    *cptflag += 1;
}

// Vérifie si le joueur a gagné en révélant toutes les cases sans mines
bool victoire(Case **tableau, int lignes, int colonnes, int nbMines)
{
    if (tableau == NULL)
    {
        printf("Pointeur nul (victoire)\n");
        return false;
    }
    int cpt = 0;
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (tableau[i][j].revealed && !tableau[i][j].hasMine)
            {
                cpt++;
            }
        }
    }
    return cpt == (lignes * colonnes) - nbMines;
}
