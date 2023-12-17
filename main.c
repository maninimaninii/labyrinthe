#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "foncts.h"
#define MAX_THREADS 4



void* resolution_limitee(void* arg) { //solution ou on limite le nombre de threads grace a mutex!

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int nombre_threads = 0;


    struct Arguments* arguments = (struct Arguments*)arg;
    if (arguments->entree.x < 0 || arguments->entree.x >= arguments->lignes || arguments->entree.y < 0 || arguments->entree.y >= arguments->colonnes || arguments->m[arguments->entree.x][arguments->entree.y] == 0) { //verifier si on est toujours dans la matrice et que ce n'est pas un mur
        pthread_exit(NULL);
    }
    if (arguments->m[arguments->entree.x][arguments->entree.y] == 3) { //condition d'arret, on se trouve sur la case sortie
        printf("Sortie trouvee en position : %d, %d!!!\n", arguments->entree.x, arguments->entree.y);
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



    pthread_t thread_id;
    int nombre_threads_crees = 0;
    for (int i = 0; i < 4 && nombre_threads_crees < MAX_THREADS; i++) {
        pthread_mutex_lock(&mutex);
        if (nombre_threads < MAX_THREADS) {
            nombre_threads++;
            pthread_mutex_unlock(&mutex);
            pthread_create(&thread_id, NULL, resolution_limitee, &argthreads[i]);
            nombre_threads_crees++;
        } else {
            pthread_mutex_unlock(&mutex);
        }
    }
    for (int i = 0; i < nombre_threads_crees; i++) {
        pthread_join(thread_id, NULL);
    }
    arguments->m[arguments->entree.x][arguments->entree.y] = 1;
    pthread_mutex_lock(&mutex);
    nombre_threads--;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}



int main() {
    FILE * fichier = fopen("resultat.txt", "r");
    int **m = CreerMatrice(7, 10, fichier);
    afficher(m, 7, 10);
    Position entree = trouveentree(m, 7,10);
    printf("\n l'entree est : %d | %d\n", entree.x, entree.y);
    Arguments args = {m,entree,7, 10 };


    pthread_t lethread;
    pthread_create(&lethread, NULL, resolution_limitee, &args);
    pthread_join(lethread, NULL);
    return 0;
}


