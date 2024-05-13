#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern void attente_aleatoire();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************/
/*
 * Fonctions 
*/
/******************************************************************************/

bool reste_des_places=2;
int nb_place_achetees = 999999;

int acheter_une_place(int *mem, int semid, int caisse_id, char * nom_film) {
    /* On protège l'accès à la shm */
    /* Reste-t-il des places libres ? */
    if (*mem > 0) {
        srand(time(0) ^ getpid());
        while (((*mem - nb_place_achetees) < 0) || (nb_place_achetees == 0)){
            nb_place_achetees = rand() % 7;
        }
        printf("%d place(s) vendue pour %s à la caisse %d, il reste désormais %d place(s) \n", nb_place_achetees, nom_film, caisse_id, *mem);
        reste_des_places = 2;
        *mem = (*mem-nb_place_achetees);
    }else if(*mem == 0){
        printf("La dernière place pour %s a été vendue à la caisse %d \n", nom_film, caisse_id);
        reste_des_places = 1;
        *mem = -1;
    }else {
        /* On écrit dans la shm */
        reste_des_places = 0;
    }

    /* On protège l'accès à la shm */
    V(semid);

    return (reste_des_places);
}


/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {
    unsigned int delais=7;

    int shmid=atoi(argv[1]);
    int semid=atoi(argv[2]);
    int caisseid=atoi(argv[3]);
    char * nom_film = argv[4];

    int *mem;
    int rand_int = 1;

    mem=attacher_segment_memoire(mem, &shmid);
    srand(time(0) ^ getpid());
    while(acheter_une_place(mem, semid, caisseid, nom_film) != 0) {
        rand_int = rand() % 3;
        while(rand_int <= 1){
            rand_int = rand() % 3;
        }

        sleep(rand_int);
    }

    printf("Caisse %d fermée \n", caisseid);
    sleep(1);
    return(0);
}
