#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ATTENTE_MAX 7

void attente_aleatoire() {

    /* Initialisation du désordre */
    srand(time(0));

    /* Attente */
    sleep(rand() % 10);

}
