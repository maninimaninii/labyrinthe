#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "foncts.h"


int trouve= 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void* resolution_illimitee(void* arg) {

    if(trouve){
        pthread_exit(NULL);
    }
    struct Arguments* arguments = (struct Arguments*)arg;
    if (arguments->entree.x < 0 || arguments->entree.x >= arguments->lignes || arguments->entree.y < 0 || arguments->entree.y >= arguments->colonnes || arguments->m[arguments->entree.x][arguments->entree.y] == 0) { //verifier si on est toujours dans la matrice et que ce n'est pas un mur
        pthread_exit(NULL);
    }
    if (arguments->m[arguments->entree.x][arguments->entree.y] == 3) { //condition d'arret, on se trouve sur la case sortie
        pthread_mutex_lock(&mutex);
        trouve = 1;
        printf("Sortie trouvee en position : %d, %d!!!\n", arguments->entree.x, arguments->entree.y);
        pthread_mutex_unlock(&mutex);

        pthread_exit(NULL);//on arrete le thread
    }
    arguments->m[arguments->entree.x][arguments->entree.y] = 0; //on met a zero pour eviter les retours en arriere infinis, meilleure version que celle avec un tableau
    struct Position directions[4] = {
          {arguments->entree.x + 1,arguments->entree.y}, //bas
          {arguments->entree.x - 1, arguments->entree.y}, //haut
          {arguments->entree.x, arguments->entree.y + 1}, //droite
          {arguments->entree.x, arguments->entree.y - 1}}; //gauche

    struct Arguments argthreads[4] = {  //on cree les arguments que l'ont va passer aux threads
        {arguments->m, directions[0], arguments->lignes, arguments->colonnes},
        {arguments->m, directions[1], arguments->lignes, arguments->colonnes},
        {arguments->m, directions[2], arguments->lignes, arguments->colonnes},
        {arguments->m, directions[3], arguments->lignes, arguments->colonnes}};



    pthread_t lesthreads[4]; //on cree un tableau contenant les 4 threads qui vont s'occuper de la partie recherche(chaque thread s'occupe d'une position)
    for (int i = 0; i < 4; i++) {
        pthread_create(&lesthreads[i], NULL, resolution_illimitee, &argthreads[i]);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(lesthreads[i], NULL);
    }
    arguments->m[arguments->entree.x][arguments->entree.y] = 1; //on remet a un apres la fin d'execution avec d'autres Positions
    pthread_exit(NULL);//on arrete le thread a la fin de l'execution
}


int main() {
    FILE * fichier = fopen("resultat.txt", "r");
    int **m = CreerMatrice(7, 10, fichier);
    afficher(m, 7, 10);
    Position entree = trouveentree(m, 7,10);
    printf("\n l'entree est : %d | %d\n", entree.x, entree.y);
    Arguments args = {m,entree,7, 10 };


    pthread_t lethread;
    pthread_create(&lethread, NULL, resolution_illimitee, &args);
    pthread_join(lethread, NULL);
    return 0;
}

