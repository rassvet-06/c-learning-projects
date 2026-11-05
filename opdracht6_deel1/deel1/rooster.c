/*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "rooster.h"

#define MAX_GROOTTE 10000

/* Functie: lees_bestand 
*  Leest inhoud van een tekstbestand in een array
* 
*  fn: bestandsnaam
*  str: pointer naar een array om inhoud op te slaan
*/
void lees_bestand(FILE *fh, char *str) {
    if (fh == NULL) {
        perror("Kan invoer niet lezen");
        exit(EXIT_FAILURE);
    }
    size_t grootte = fread(str, 1, MAX_GROOTTE - 1, fh);
   //  fclose(fh);
    if (grootte == MAX_GROOTTE - 1) {
        fprintf(stderr, "De invoer was te lang.\n");
        exit(EXIT_FAILURE);
    }
    str[grootte] = '\0';
}

struct rooster_data {
    int breedte;     // number of columns
    int hoogte;      // number of rows
    toestand staat;  // current game staat (BEGIN, AAN_HET_SPELEN, GEWONNEN, VERLOREN)
    char *velden;    // 1D array of characters containing the maze layout
};

/* Maak een rooster op basis van de data in de gegeven stream.

   fh: de stream waaruit het doolhof gelezen moet worden.

   Uitvoer: als alles goed gaat, een pointer naar een rooster (die op de heap is
            gealloceerd), dat overeenkomt met de gegeven beschrijving.
            De begintoestand is BEGIN.

            Als de beschrijving niet consistent is (bijvoorbeeld
            niet alle rijen zijn even lang, of er klopt iets anders niet), of
            als niet voldoende geheugen kan worden gereserveerd, dan wordt
            NULL teruggegeven. (In dat geval blijft geen gereserveerd geheugen
            achter.)
*/
rooster *rooster_lees(FILE *fh) {

   rooster *rp = malloc(sizeof(rooster));
   if (rp == NULL) {
      perror("malloc");
      return NULL;
   }

   rp->velden = malloc(MAX_GROOTTE);
   if (rp->velden == NULL) {
      perror("malloc");
      free(rp);
      return NULL;
   }

   lees_bestand(fh, rp->velden);

   rp->hoogte = 0, rp->breedte = 0;
   int kolommen_in_rij = 0;

   for (int i = 0; rp->velden[i] != '\0'; i++) {
      printf("%c", rp->velden[i]);
      if (rp->velden[i] == '\n') {
         rp->hoogte++;
         if (rp->breedte == 0) {
               rp->breedte = i;
         }
      if (kolommen_in_rij != rp->breedte) {
         printf("Ongelijk aantal kolommen in hoogte\n");
         free(rp->velden);
         free(rp);
         return NULL;
      }
         kolommen_in_rij = -1;
      }
      kolommen_in_rij++;
   }
   // printf("Aantal hoogte: %d\n", rp->hoogte);
   // printf("Aantal kolommen: %d\n", rp->breedte);

   rp->staat = BEGIN;

   return rp;
}

/* Vraag de huidige toestand van het spel op.

   rp: een pointer naar het rooster.

   Uitvoer: de toestand.
*/
toestand rooster_vraag_toestand(const rooster *rp) {
   return rp->staat;
}


/* Verander de huidige toestand van het spel.

   rp: een pointer naar het rooster.
   t:  de nieuwe toestand.
*/
void rooster_zet_toestand(rooster *rp, toestand t) {
   rp->staat = t;
}

/* Geef alle resources vrij die zijn gealloceerd voor een rooster.
   De rooster pointer is na aanroep van deze functie niet meer bruikbaar.

   rp: een pointer naar het rooster.
*/
void rooster_klaar(rooster *rp) {
   if (rp == NULL) {
      return;
   }
   free(rp->velden);
   free(rp);
}

/* Vraag de breedte van het rooster op, dat wil zeggen, het aantal kolommen.

   rp: een pointer naar het rooster.

   Uitvoer: de breedte.
*/
int rooster_breedte(const rooster *rp) {
    return rp->breedte;
}

/* Vraag de hoogte van het rooster op, dat wil zeggen, het aantal rijen.

   rp: een pointer naar het rooster.

   Uitvoer: de hoogte.
*/
int rooster_hoogte(const rooster *rp) {
    return rp->hoogte;
}

/* Kijk of de gegeven positie binnen het rooster valt.

   rp:  een pointer naar het rooster.
   x,y: de positie.

   Uitvoer: 1 als de positie binnen het rooster valt, anders 0.
*/
int rooster_bevat(const rooster *rp, int x, int y) {
   // return 1 if inside, else 0
    if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
        return 1;
    }
    return 0;
}

/* Kijk welk object er staat op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.

   Uitvoer: het object op die positie, of '\0' als de positie buiten het
            rooster valt.
*/
char rooster_kijk(const rooster *rp, int x, int y) {
    // TODO:
    // if outside, return '\0'
    // else return rp->velden[y * (rp->breedte + 1) + x]
      if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
         return rp->velden[y * (rp->breedte + 1) + x];
      }
    return '\0';
}

/* Schrijf een bepaald object op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.
   c  : het object.

   Effect: als (x,y) binnen het rooster ligt, wordt het object op
           de opgegeven positie geplaatst, anders verandert er niets.

   Uitvoer: 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, char c) {
    // TODO:
    // if inside bounds → write character → return 1
    // else return 0
      if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
         int i = y * (rp->breedte + 1) + x;
         rp->velden[i] = c;
         return 1;
      }
    return 0;
}

/* Zoek een bepaald object in het rooster, en geef de coordinaten van het
   object terug via de gegeven pointers. Let op: als er meerdere objecten van
   het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
   de positie wordt gevonden.

   rp : een pointer naar het rooster
   c  : het object dat moet worden gezocht
   x,y: pointers naar de geheugenlocaties waar de gevonden positie moet worden
        geschreven.

   Uitvoer: via de pointers x en y. Als het object niet wordt gevonden worden
            *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, char c, int *x, int *y) {
    // TODO:
    // Loop over all positions
    // if found → set *x, *y and return
    // if not found → set *x = -1, *y = -1
      for (int row = 0; row < rp->hoogte; row++) {
         for (int col = 0; col < rp->breedte; col++) {
            int i = row * (rp->breedte + 1) + col;
            if (rp->velden[i] == c) {
               *x = col;
               *y = row;
               return;
            }
         }
      }
      *x = -1;
      *y = -1;
}
