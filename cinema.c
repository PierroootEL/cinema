#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shm_const.h"

/**
* Fonctions externes
*/
extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();
extern int creer_segment_memoire();


/**
* Programme principale
*/
int main(int argc, char *argv[]) {

    int code_retour_fin_entree;
    int code_retour_fin_sortie;

    // Nombre de caisses
    int nombre_caisses_cinema; /* Pour écriture dans la shm */
    char *nombre_caisses_cinema_str; /* Pour conversion du semid (int) en chaine */

    // Nom du film
    char *nom_film_cinema;

    // Nombre de places
    int nombre_places_cinema;
    char *nombre_places_cinema_str;

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int *mem; /* Adresse du segment de mémoire partagée */
    int creation_time;
    int arr_size;

    char param_gnome_terminal[80];

    /** Vérification des arguments */
    if (argc != 4) {
        fprintf(stderr, "Merci de renseigner les arguments necassaires sur le programme : %s", argv[0]);
        return (3);
    }

    /* Création sémaphore */
    creer_initialiser_semaphore(CLE_SEM, &semid);
    /* Création segment de mémoire partagé */
    creer_segment_memoire(CLE_SHM, &shmid);

    /* Attachement du segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);

    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);

    /** Gestion des arguments en entrée de script */
    // Nombre de caisses
    nombre_caisses_cinema_str = argv[1];
    nombre_caisses_cinema = atoi(nombre_caisses_cinema_str);

    // Nom du film
    nom_film_cinema = argv[2];

    // Nombre de places
    nombre_places_cinema_str = argv[3];
    nombre_places_cinema = atoi(nombre_places_cinema_str);

    /* Pas besoin de sémaphore on est seul :-) */
    *mem = nombre_places_cinema;

    if (nombre_caisses_cinema < 1) {
        fprintf(stderr, "Merci de renseigner un nombre de caisse(s) supérieur(s) à 0");
        return (3);
    }

    if (nombre_places_cinema < 1) {
        fprintf(stderr, "Merci de renseigner un nombre de place(s) supérieur(s) à 0");
        return (3);
    }

    // Variable du for ( pour les caisses du cinema )
    pid_t caisses[nombre_caisses_cinema];
    pid_t pere_pid = getpid();
    int i, j = 0;

    printf("Pid du programme père : %d \n", pere_pid);
    printf("Nombre de caisses : %d \n", nombre_caisses_cinema);
    printf("Nombre de place de cinéma à vendre : %d \n", nombre_places_cinema);
    creation_time = nombre_caisses_cinema;

    for (i = 1; i <= nombre_caisses_cinema; i++) {
        if (getpid() == pere_pid) {
            caisses[i] = fork();
            if (caisses[i] == -1) {
                printf("Erreur lors de la création du fils n°%d \n", i);
                return 1;
            } else if (caisses[i] == 0) {
                printf("Caisse %d créée avec succès \n", i);
                char i_str[20];
                sprintf(i_str, "%d", i);
                sleep(creation_time);
                creation_time -= 1;
                execl("caisse", "caisse", shmid_str, semid_str, i_str, NULL);
            }
            sleep(1);
        }

    }

    waitpid(caisses[nombre_caisses_cinema], 0, 0);
    printf("Fin du programme, la séance est complète ! \n");
    return 0;
}