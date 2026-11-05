/*
*/
#define MAX_GROOTTE 10000

#include <stdlib.h>
#include <stdio.h>

/* Functie: lees_bestand 
*  Leest inhoud van een tekstbestand in een array
* 
*  fn: bestandsnaam
*  str: pointer naar een array om inhoud op te slaan
*
*  Side effect: de array wordt gevuld met de inhoud van het bestand
*/


void lees_bestand(FILE *fh, char *str) {
    if (fh == NULL) {
        fprintf(stderr, "Fout: bestand is niet geopend.\n");
        exit(EXIT_FAILURE);
    }

    size_t grootte = fread(str, 1, MAX_GROOTTE - 1, fh);

    if (ferror(fh)) {
        perror("Fout bij lezen van bestand");
        exit(EXIT_FAILURE);
    }

    if (grootte == MAX_GROOTTE - 1) {
        fprintf(stderr, "De invoer was te lang.\n");
        exit(EXIT_FAILURE);
    }

    str[grootte] = '\0';
}