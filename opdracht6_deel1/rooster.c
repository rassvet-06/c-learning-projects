/*
*/
/*
*  Doel rooster.c: Implementatie van het rooster datatype en bijbehorende
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

#define MAX_GROOTTE 10000

/* Functie: lees_bestand 
*  Leest inhoud van een tekstbestand in een array
* 
*  fn: bestandsnaam
*  str: pointer naar een array om inhoud op te slaan
*/
int lees_bestand(FILE *fh, char *str) {

   if (fh == NULL) {
      return -1;
   }

   size_t grootte = fread(str, 1, MAX_GROOTTE - 1, fh);

   if (grootte == MAX_GROOTTE - 1) {
      /* input too long */
      return -1;
   }

   if (ferror(fh)) {
      return -1;
   }

   str[grootte] = '\0';
   return 0;
}

/* Structuur voor het rooster
*  Structuur bevat definities voor het rooster
*
*  breedte: aantal kolommen
*  hoogte: aantal rijen
*  staat: huidige toestand van het spel
*  velden: 1d array van chars met de inhoud van het rooster
* 
*  Uitvoer: pointer naar het aangemaakte rooster, of NULL bij fouten
*/
struct rooster_data {
    int breedte;    
    int hoogte;      
    toestand staat;  
    char *velden;    
};

/* functie: rooster_lees
*  Leest een rooster uit een bestand
*  en retourneert een pointer naar het aangemaakte rooster
*
*  fh: file handle van het bestand
*  rp->velden: pointer naar een array met de inhoud van het rooster
*  rp->breedte: breedte van het rooster
*  rp->hoogte: hoogte van het rooster
*  rp->staat: huidige toestand van het spel
*
*  Uitvoer: pointer naar het aangemaakte rooster, of NULL bij fouten
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

   if (lees_bestand(fh, rp->velden) != 0) {
      free(rp->velden);
      free(rp);
      return NULL;
   }

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
         free(rp->velden);
         free(rp);
         return NULL;
      }
         kolommen_in_rij = -1;
      }
      kolommen_in_rij++;
   }

   rp->staat = BEGIN;

   return rp;
}

/* functie: rooster_vraag_toestand
*  Vraagt de huidige toestand van het spel op
*
*  rp: een pointer naar het rooster
*/
toestand rooster_vraag_toestand(const rooster *rp) {

   return rp->staat;
}


/* functie: rooster_zet_toestand
*  Zet de huidige toestand van het spel
*
*  rp: een pointer naar het rooster
*  t: de nieuwe toestand
*/
void rooster_zet_toestand(rooster *rp, toestand t) {

   rp->staat = t;
}

/* functie: rooster_klaar
*  Maakt het rooster vrij
*
*  rp: een pointer naar het rooster
*/
void rooster_klaar(rooster *rp) {

   if (rp == NULL) {
      return;
   }

   free(rp->velden);
   free(rp);
}

/* functie: rooster_breedte
*  Vraagt de breedte van het rooster op
*
*  rp: een pointer naar het rooster
*
*  Uitvoer: de breedte
*/
int rooster_breedte(const rooster *rp) {

    return rp->breedte;
}

/* functie: rooster_hoogte
*  Vraagt de hoogte van het rooster op
*
*  rp: een pointer naar het rooster
*
*  Uitvoer: de hoogte
*/
int rooster_hoogte(const rooster *rp) {

    return rp->hoogte;
}

/* functie: rooster_bevat
*  Controleert of een bepaalde positie binnen de grenzen van het rooster ligt
*
*  rp:  een pointer naar het rooster
*  x: de x-coördinaat
*  y: de y-coördinaat
*
*  Uitvoer: 1 als de positie binnen het rooster valt, anders 0
*/
int rooster_bevat(const rooster *rp, int x, int y) {
   
    if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
        return 1;
    }

    return 0;
}

/* functie: rooster_kijk
*  Kijk welk object er staat op een bepaalde positie in het rooster
*
*  rp : een pointer naar het rooster
*  x: de x-coördinaat
*  y: de y-coördinaat
*
*  Uitvoer: het object op die positie, 
*  of '\0' als de positie buiten het rooster valt
*/
char rooster_kijk(const rooster *rp, int x, int y) {

      if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
         return rp->velden[y * (rp->breedte + 1) + x];
      }

    return '\0';
}

/* functie: rooster_plaats
*  Plaatst een bepaald object op een bepaalde positie in het rooster
*
*  rp : een pointer naar het rooster
*  x: de x-coördinaat
*  y: de y-coördinaat
*  c  : het object
*  
* Uitvoer: 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, char c) {

      if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
         int i = y * (rp->breedte + 1) + x;
         rp->velden[i] = c;
         return 1;
      }
      
    return 0;
}

/* functie: rooster_zoek
*  Zoekt de positie van een bepaald object in het rooster
*
*  rp: een pointer naar het rooster
*  c : het object
*  x: pointer naar een int voor de x-coördinaat
*  y: pointer naar een int voor de y-coördinaat
*
*  Uitvoer: via de pointers x en y. Als het object niet wordt gevonden worden
*           *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, char c, int *x, int *y) {
    
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
