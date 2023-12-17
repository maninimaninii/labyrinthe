#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "foncts.h"



int chemincorr[10][10] = {0}; //matrice ta3 le chemin


   int resolution(int **m, int x, int y,int lignes, int colonnes, Position *s) { //la magie le shour
    if (m[x][y] == 3) { //si on est sur la sortie
        chemincorr[x][y] = 1; //on met la fin du chemin � 1
        (*s).x = x; //on recup les coordonees du point
        (*s).y = y;
        return 1;
    }

    if (m[x][y] == 0 || chemincorr[x][y] == 1) { // si c'est un mur pour le 0 ||| quant � la deuxieme comparaison, elle sert � �viter les boucles infinies (retour en arriere puis avant puis arriere)
        return 0;
    }

    chemincorr[x][y] = 1;

    if (x + 1 < lignes && resolution(m, x + 1, y,lignes, colonnes, s)) { // bas
        return 1;
    }

    if (y + 1 < colonnes && resolution(m, x, y + 1,lignes, colonnes, s)) { // droite
        return 1;
    }

    if (x - 1 >= 0 && resolution(m, x - 1, y,lignes, colonnes, s)) { // haut
        return 1;
    }

    if (y - 1 >= 0 && resolution(m, x, y - 1, lignes, colonnes, s)) { // gauche
        return 1;
    }

    chemincorr[x][y] = 0; //si aucune des solutions disponibles, on rebrousse chemin grace au backtracking et on remet � 0 car il ne fera pas partie du chemin
    return 0;
}




int main()
{

    FILE * fichier = fopen("resultat.txt", "r");
    int **m = CreerMatrice(7, 10, fichier);
    afficher(m, 7, 10);
    Position entree = trouveentree(m, 7,10);
    printf("\n l'entree est : %d | %d\n", entree.x, entree.y);
    Position s;
    if (resolution(m, entree.x, entree.y,7,10, &s)) {
        printf("Voila le chemin:\n");
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 10; j++) {
                printf("%d ", chemincorr[i][j]);
            }
            printf("\n");
        }
        printf("La solution est donc � la ligne %d, colonne %d ", s.x, s.y);

    }
}

