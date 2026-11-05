/*
*/
/*
*  Doel spel.c: Implementatie van het spel en bijbehorende 
*  functies:
* - toon_rooster (toont het rooster met ncurses)
* - beweeg (beweegt de speler in een bepaalde richting)
* - speel (speelt het spel tot het afgelopen is)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include <ncurses.h>

#include "rooster.h"

/* functie: toon_rooster
*  Toont het rooster op het scherm met behulp van ncurses
*
*  rp: een pointer naar het rooster
*  Side effect: het scherm wordt aangepast
*  om het rooster weer te geven.
*/
void toon_rooster(rooster *rp) {

    clear();

    int breedte = rooster_breedte(rp);
    int hoogte = rooster_hoogte(rp);

    for (int y = 0; y < hoogte; y++) {

        for (int x = 0; x < breedte; x++) {
            char c = rooster_kijk(rp, x, y);
            mvaddch(y, x, c);
        }
    }

    refresh();
}

/* functie: beweeg
*  Beweegt de speler in een bepaalde richting
*
*  rp: een pointer naar het rooster
*  dx: verandering in x-richting (-1;0); (1;0)
*  dy: verandering in y-richting (0;-1); (0;1) 
* 
*  Side effect: het rooster wordt aangepast op basis van
*               de beweging van de speler.
*/
void beweeg(rooster *rp, int dx, int dy) {
    
    int nieuwe_positie_x = -1;
    int nieuwe_positie_y = -1;
    int speler_x = -1;
    int speler_y = -1;

    rooster_zoek(rp, '*', &speler_x, &speler_y);    
    nieuwe_positie_x = speler_x + dx;
    nieuwe_positie_y = speler_y + dy;

    if (!rooster_bevat(rp, nieuwe_positie_x, nieuwe_positie_y)) {
        return;
    }

    if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == ' ') {
        rooster_plaats(rp, speler_x, speler_y, ' ');
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
    }

    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == '$') {
        rooster_plaats(rp, speler_x, speler_y, ' ');            
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
        rooster_zet_toestand(rp, GEWONNEN);
    }

    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == 'X') {
        rooster_plaats(rp, speler_x, speler_y, ' ');
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
        rooster_zet_toestand(rp, VERLOREN);
    }

    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == '#') {
        return;
     }
    
     return;
}
/* functie: speel
*  Speelt het spel tot het afgelopen is
*
*  rp: een pointer naar het rooster
*  Side effect: het rooster wordt aangepast op basis van
*               de beweging van de speler.
 */
void speel(rooster *rp) {

    int ch;

    assert(rp != NULL);
    assert(rooster_vraag_toestand(rp) == BEGIN);
    rooster_zet_toestand(rp, AAN_HET_SPELEN);

    toon_rooster(rp);

    while (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {
        
        ch = getch();

        switch (ch) {
        case KEY_UP:
        case 'W':
            beweeg(rp, 0, -1);
            break;

        
        case KEY_LEFT:
        case 'A':
            beweeg(rp, -1, 0);
            break;

        case KEY_DOWN:
        case 'S':
            beweeg(rp, 0, 1);
            break;

        case KEY_RIGHT:
        case 'D':
            beweeg(rp, 1, 0);
            break;

        default:
            break;
        }
        toon_rooster(rp);
    }
    if (rooster_vraag_toestand(rp) == GEWONNEN) {
        mvprintw(rooster_hoogte(rp) + 1, 0, "Hip Hip! Je hebt gewonnen!");
        rooster_zet_toestand(rp, GEWONNEN);

    } else if (rooster_vraag_toestand(rp) == VERLOREN) {
        mvprintw(rooster_hoogte(rp) + 1, 0, "Helaas, je hebt verloren.");
        rooster_zet_toestand(rp, VERLOREN);
    }

    refresh();
    return;
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
