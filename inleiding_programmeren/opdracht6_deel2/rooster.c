/*
*/

/* Doet: Implementatie van het rooster en bijbehorende
*  functies:
*  - rooster_lees (doet bestand inlezen)
*  - rooster_vraag_toestand (vraagt huidige toestand op)
*  - rooster_zet_toestand (zet huidige toestand)
*  - rooster_klaar (maakt rooster vrij)
*  - rooster_breedte (vraagt breedte op)
*  - rooster_hoogte (vraagt hoogte op)
*  - rooster_bevat (controleert of een positie binnen het rooster ligt)
*  - rooster_kijk (kijkt naar de inhoud van een specifieke positie)
*  - rooster_plaats (plaatst een waarde op een specifieke positie)
*  - rooster_zoek (zoekt naar een waarde in het rooster)
*  - lees_bestand (leest een bestand in)
*  - struct rooster_data (structuur voor het rooster)
*  - enum toestand (mogelijke toestanden van het spel)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "rooster.h"
#include "lees_bestand.h"

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

   rp->velden = malloc(MAX_GROOTTE);

   lees_bestand(fh, rp->velden);

   rp->hoogte = 0;
   rp->breedte = 0;
   int kolommen_in_rij = 0;

   for (int i = 0; rp->velden[i] != '\0'; i++) {
      if (rp->velden[i] == '\n') {
         rp->hoogte++;
         if (rp->breedte == 0)
             rp->breedte = i;
         if (kolommen_in_rij != rp->breedte) {
             fprintf(stderr, "Ongelijk aantal kolommen in hoogte\n");
             free(rp->velden);
             free(rp);
             return NULL;
         }
         kolommen_in_rij = -1;
      }
      kolommen_in_rij++;
   }

   rp->staat = BEGIN;
   return rp; // ✅ add this line
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
