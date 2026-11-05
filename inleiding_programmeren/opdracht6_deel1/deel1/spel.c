/*
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

#include "rooster.h"

/* Toont het gegeven rooster met ncurses.

   rp: een pointer naar het rooster.
*/
void toon_rooster(rooster *rp) {
    // TO DO: schrijf hier je eigen implementatie
}

/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp   : een pointer naar het rooster
   dx,dy: de richting waarin de speler probeert te bewegen. De vier mogelijk-
          heden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor resp.
          links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, int dx, int dy) {
    // TO DO: schrijf hier je eigen implementatie
}

/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is.
 */
void speel(rooster *rp) {
    // TO DO: schrijf hier je eigen implementatie.
}

int main(int argc, char *argv[]) {
    // 1. Controleer dat er een doolhofbestand is opgegeven op de command line.
    if (argc != 2) {
        fprintf(stderr, "gebruik: ./spel assets/voorbeeld_doolhof.txt\n");
        return 1;
    }

    // 2. Open het doolhofbestand en lees het rooster.
    FILE *fh = fopen(argv[1], "r");
    if (fh == NULL) {
        perror("main");
        return 1;
    }
    rooster *rp = rooster_lees(fh);
    fclose(fh);

    // 3. Bepaal of het lezen van het rooster is gelukt.
    if (rp == NULL) {
        fprintf(stderr, "Kan rooster niet maken.\n");
        return 1;
    }

    // 4. Initialiseer ncurses
    initscr();
    cbreak();             // zodat je kunt onderbreken met Ctrl+C
    keypad(stdscr, TRUE); // luister ook naar extra toetsen zoals pijltjes
    noecho();             // druk niet de letters af die je intypt

    // 5. Speel het spel.
    speel(rp);

    // 6. Sluit af.
    getch(); // Optioneel, om te zorgen dat het spel niet meteen stopt.
    rooster_klaar(rp);
    endwin();
    return 0;
}
