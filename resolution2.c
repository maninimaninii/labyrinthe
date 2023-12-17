#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "foncts.h"


//autre version de la resolution, celle ci est plus efficace mais on n'obtient pas le chemin vers la sortie.


int trouve = 0; // int qui fait office de booleen qui permet de determiner si la sortie a deja ete trouvee.


   int resolution2(int **m, int x, int y,int lignes, int colonnes, Position *s) { //la magie le shour

       if(trouve){
        return 0;
       }
    if (m[x][y] == 3) { //si on est sur la sortie
        trouve = 1;
        (*s).x = x; //on recup les coordonees du point
        (*s).y = y;

        printf("sortie trouvee en %d ,  %d\n", x, y);
        return 1;
    }

    if (m[x][y] == 0) {
        return 0;
    }

    m[x][y] = 0;

    if (x + 1 < lignes && resolution2(m, x + 1, y,lignes, colonnes, s)) { // bas
        return 1;
    }

    if (y + 1 < colonnes && resolution2(m, x, y + 1,lignes, colonnes, s)) { // droite
        return 1;
    }

    if (x - 1 >= 0 && resolution2(m, x - 1, y,lignes, colonnes, s)) { // haut
        return 1;
    }

    if (y - 1 >= 0 && resolution2(m, x, y - 1, lignes, colonnes, s)) { // gauche
        return 1;
    }

    m[x][y] = 1;
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
    resolution2(m, entree.x, entree.y,7,10, &s);
    }

