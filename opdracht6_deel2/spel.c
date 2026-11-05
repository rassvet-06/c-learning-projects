/*
*/
/*
*  Doel spel.c: Implementatie van het spel en bijbehorende 
*  functies:
* - toon_rooster (toont het rooster met ncurses)
* - beweeg (beweegt de speler in een bepaalde richting)
* - speel (speelt het spel tot het afgelopen is)
*  
*   rp: een pointer naar het rooster
*   dx: verandering in x-richting (-1;0); (1;0)
*   dy: verandering in y-richting (0;-1); (0;1)
*   ch: karakter die is ingedrukt
*   keuze: keuze in het menu
*   keuze_doolhof: keuze voor het doolhof
*   pad: bestandsnaam van het doolhof
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

    // als speler buiten het rooster beweegt, doe niets
    if (!rooster_bevat(rp, nieuwe_positie_x, nieuwe_positie_y)) {
        return;
    }

    // als speler beweegt naar een lege plek, doel, of val
    if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == ' ') {
        rooster_plaats(rp, speler_x, speler_y, ' ');
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
    }

    // als speler beweegt naar het doel 
    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == '$') {
        rooster_plaats(rp, speler_x, speler_y, ' ');            
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
        rooster_zet_toestand(rp, GEWONNEN);
    }

    // als speler beweegt naar een val
    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == 'X') {
        rooster_plaats(rp, speler_x, speler_y, ' ');
        rooster_plaats(rp, nieuwe_positie_x, nieuwe_positie_y, '*');
        rooster_zet_toestand(rp, VERLOREN);
    }

    // als speler beweegt naar een muur, doe niets
    else if (rooster_kijk(rp, nieuwe_positie_x, nieuwe_positie_y) == '#') {
        return;
     }
    
    return;
}

/* functie: speel
*  Speelt het spel tot het afgelopen is
*
*  rp: een pointer naar het rooster
*
*  Side effect: het rooster wordt aangepast op basis van
*               de beweging van de speler.
*               Het spel eindigt als de speler wint, verliest,
*               of stopt.
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
        
        case 'Q':
        case 'q':
            rooster_zet_toestand(rp, GESTOPT);   
            clear(); 
            mvprintw(rooster_hoogte(rp) + 1, 0, "Je hebt het spel verlaten.\nDruk op een toets om terug te keren naar het menu.");
            getch();
            return;

        default:
            break;
        }

        toon_rooster(rp);
    }

    // Spel is afgelopen, toon resultaat
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

/* Hoofdfunctie main
*
 * Toont menu met drie keuzes:
 * nieuw spel, instructies, afsluiten
 * Gebruiker kan een doolhof kiezen en het spel starten
 * Het spel wordt gespeeld met ncurses
 * Het programma blijft gaan tot de gebruiker afsluit (q)
 * Gebruiker kan onjuste invoer geven, dan krijgt hij een keuze
 * om opnieuw te proberen
 * 
 * Input: keuze van de gebruiker
 * Output: het gekozen doolhof wordt geladen en het spel gestart
 *        of instructies worden getoond
 *       of het programma wordt afgesloten
 *      of foutmelding bij ongeldige invoer
 * 
 * Side effect: het scherm wordt aangepast op basis van de keuze van de gebruiker
 */
int main(void)
{
    int keuze = 0;

    do {
        printf("\n=== MENU LABYRINTSPELLETJE ===\n");
        printf("1. Nieuw spelletje starten\n");
        printf("2. Instructies\n");
        printf("3. Afsluiten\n");
        printf("Voer uw keuze in: ");

        if (scanf("%d", &keuze) != 1) {
            printf("Onjuiste invoer! Probeer opnieuw.\n");
            while (getchar() != '\n'); 
            continue;
        }

        // keuze menu
        switch (keuze) {
            case 1: {
                // nieuw spel
                char pad[256];
                int keuze_doolhof = 0;

                printf("\n--- Kies een doolhof ---\n");
                printf("1. Makkelijk (assets/voorbeeld_doolhof.txt)\n");
                printf("2. Middel (assets/voorbeeld_doolhof_2.txt)\n");
                printf("3. Moeilijk (assets/voorbeeld_doolhof_3.txt)\n");
                printf("Voer uw keuze in: ");
                scanf("%d", &keuze_doolhof);

                // doolhof bepalen
                if (keuze_doolhof == 1) {
                    strcpy(pad, "assets/voorbeeld_doolhof_1.txt");
                    printf("Makkelijk doolhof wordt geladen...\n");
                } else if (keuze_doolhof == 2) {
                    strcpy(pad, "assets/voorbeeld_doolhof_2.txt");
                    printf("Medium doolhof wordt geladen...\n");
                } else if (keuze_doolhof == 3) {
                    strcpy(pad, "assets/voorbeeld_doolhof_3.txt");
                    printf("Moeilijk doolhof wordt geladen...\n");
                } else {
                    printf("Onjuiste keuze, standaard doolhof wordt geladen.\n");
                    strcpy(pad, "assets/voorbeeld_doolhof_1.txt");
                }

                // doolhof bestand openen
                FILE *fh = fopen(pad, "r");
                if (fh == NULL) {
                    perror("Fout bij openen van het file");
                    break;
                }

                // rooster inlezen
                rooster *rp = rooster_lees(fh);
                fclose(fh);

                if (rp == NULL) {
                    fprintf(stderr, "Fout: kon rooster niet inlezen.\n");
                    break;
                }

                // ncurses initialiseren
                initscr();
                cbreak();
                keypad(stdscr, TRUE);
                noecho();

                speel(rp);

                getch();
                rooster_klaar(rp);
                endwin();

                break;
            }

            case 2:
                // instructies tonen
                printf("\n--- INSTRUCTIES ---\n");
                printf("Welkom bij het labyrintspel!\n");
                printf("Gebruik pijltjestoetsen of WASD om te bewegen.\n");
                printf("Doel is $. Bereik die zonder muren # te raken\n");
                printf("Als je op X stapt dan val je en dat stopt het spel.\n");
                printf("* is de speler.\n");
                printf("Veel succes!\n");
                break;

            case 3:
                // afsluiten
                printf("\nEinde van het spel. Tot de volgende keer!\n");
                break;

            default:
                printf("Onjuiste keuze! Kijk naar de opties en probeer opnieuw.\n");
                break;
        }

    } while (keuze != 3);

    return 0;
}
