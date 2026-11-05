
/* Conway's Game of Life
* FUNCTIES:
* initialiseer_grid: matrix leeg maken en
* vullen met een gekozen startpatroon
* toon(grid, generatie): raster afdrukken
* met '*' (levend) en '.' (dood)
* kopie(bron, doel): volledige rasterkopie
* buren(grid, x, y): tel 8-buurs 
* stap(oud, nieuw): bereken volgende generatie 
*/ 

#include <stdio.h>
#include <stdlib.h>

#define MAX_OPDRACHT_LEN 100
#define BREEDTE 79
#define HOOGTE 40
#define LEVEND 1
#define DOOD 0

/* Functie initialiseer_grid: 
* zet beginpatroon in het grid 
* tekent patroon van de gegeven voorbeeld
* grid: matrix om te vullen
*/
void initialiseer_grid(int grid[HOOGTE][BREEDTE], int patroon)
{
    for (int rij = 0; rij < HOOGTE; ++rij) {
        for (int kolom = 0; kolom < BREEDTE; ++kolom) {
            grid[rij][kolom] = 0;
        }
    }

    if (patroon == 1) {
        grid[1][2] = 1;
        grid[2][2] = 1;
        grid[2][3] = 1;
        grid[3][2] = 1;
        grid[3][3] = 1;
        grid[4][3] = 1;
    } else if (patroon == 2) {
        grid[1][2] = 1;
        grid[2][3] = 1;
        grid[3][1] = 1;
        grid[3][2] = 1;
        grid[3][3] = 1;
    } else if (patroon == 3) {
        grid[1][25] = 1;

        grid[2][23] = 1;
        grid[2][25] = 1;

        grid[3][13] = 1;
        grid[3][14] = 1;
        grid[3][21] = 1;
        grid[3][22] = 1;
        grid[3][35] = 1;
        grid[3][36] = 1;

        grid[4][12] = 1;
        grid[4][16] = 1;
        grid[4][21] = 1;
        grid[4][22] = 1;
        grid[4][35] = 1;
        grid[4][36] = 1;

        grid[5][1] = 1;
        grid[5][2] = 1;
        grid[5][11] = 1;
        grid[5][17] = 1;
        grid[5][21] = 1;
        grid[5][22] = 1;

        grid[6][1] = 1;
        grid[6][2] = 1;
        grid[6][11] = 1;
        grid[6][15] = 1;
        grid[6][17] = 1;
        grid[6][18] = 1;
        grid[6][23] = 1;
        grid[6][25] = 1;

        grid[7][11] = 1;
        grid[7][17] = 1;
        grid[7][25] = 1;

        grid[8][12] = 1;
        grid[8][16] = 1;

        grid[9][13] = 1;
        grid[9][14] = 1;
    }
}

/* Functie toon geeft eerst de huidge generatie weer
 * en drukt dan het grid af met lettertekens
 * grid: te tonen matrix
 * generatie: huidige generatie
 */
void toon(const int grid[HOOGTE][BREEDTE], int generatie)
{
    printf("\nGeneratie %d:\n", generatie);
    for (int rij = 0; rij < HOOGTE; ++rij) {
        for (int kolom = 0; kolom < BREEDTE; ++kolom) {
            if (grid[rij][kolom] == 1) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

/* Functie kopie:
* kopieert de inhoud van het array bron_grid 
* naar doel_grid.
* na berekenen van nieuwe generatie 
* willen wij oude generatie overschrijven
* bron_grid: de originele matrix
* doel_grid: de nieuwe matrix
*/
void kopie(const int bron_grid[HOOGTE][BREEDTE], int doel_grid[HOOGTE][BREEDTE])
{
    for (int rij = 0; rij < HOOGTE; ++rij) {
        for (int kolom = 0; kolom < BREEDTE; ++kolom) {
            doel_grid[rij][kolom] = bron_grid[rij][kolom];
        }
    }
}

/* Functie buren:
* aantal levende cellen rondom (x,y). De cel zelf telt niet mee
* reden: grenscontrole 
* grid: om  rijen en kolommen te checken
* x: kolom
* y: rij
* burenaantal: aantal levende buren
*/
int buren(const int grid[HOOGTE][BREEDTE], int x, int y)
{
    int burenaantal = 0;
    int rij = y;
    int kolom = x;
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, 
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (int d = 0; d < 8; ++d) {
        int buur_rij = rij + directions[d][0];
        int buur_kolom = kolom + directions[d][1];

        if (buur_rij < 0 || buur_rij == HOOGTE || buur_kolom < 0 || buur_kolom == BREEDTE) {
            continue;
        }

        if (grid[buur_rij][buur_kolom] == 1) {
            ++burenaantal;
        }
    }

    return burenaantal;
}

/* Functie stap
* berekent nieuwe generatie in nieuw_grid 
* op basis van oud_grid
* Levend overleeft bij 2 of 3 buren 
* Wordt geboren(levend) bij 3 buren exact
* variabele oud_grid Huidige generatie
* variabele nieuw_grid Volgende generatie
*/
void stap(const int oud_grid[HOOGTE][BREEDTE], int nieuw_grid[HOOGTE][BREEDTE])
{
    for (int rij = 0; rij < HOOGTE; ++rij) {
        for (int kolom = 0; kolom < BREEDTE; ++kolom) {
            int burenaantal = buren(oud_grid, kolom, rij);
            int rabbit = 0;
            if (oud_grid[rij][kolom] == 1) {
                /* check als konijn blijft leven of gaat dood */
                if (burenaantal == 2 || burenaantal == 3) {
                    rabbit = 1;
                }
            } else {
                /* checken als er een nieuwe konijn geboren moet worden */
                if (burenaantal == 3) {
                    rabbit = 1;
                }
            }
            nieuw_grid[rij][kolom] = rabbit;
        }
    }
}

/* Hoofdprogramma.
* in lus:
* toont begin situatie voor gekozen patroon
* vraagt aantal generaties 
* en sluit het programma af
*/
#ifndef COMPILE_MAIN
int main(int argc, char *argv[])
{
    char buffer[MAX_OPDRACHT_LEN];
    int grid[HOOGTE][BREEDTE];
    int nieuw_grid[HOOGTE][BREEDTE];

    int patroon = argc == 1 ? 1 : atoi(argv[1]);

    initialiseer_grid(grid, patroon);
    int generatie = 0;

    printf("Conway's Game of life\n\n");
    toon(grid, generatie);
    while (1) {
        printf("\nGeef een opdracht:\n");
        char *res = fgets(buffer, MAX_OPDRACHT_LEN, stdin);
        if (res == NULL) {
            /* Er is waarschijnlijk op Ctrl+D gedrukt. */
            printf("Programma wordt beëindigd.\n");
            break;
        }
        int aantal_generaties = buffer[0] == '\n' ? 1 : atoi(buffer);
        for (int i = 0; i < aantal_generaties; i++) {
            stap(grid, nieuw_grid);
            kopie(nieuw_grid, grid);
        }
        generatie += aantal_generaties;
        toon(grid, generatie);
    }
    return 0;
}
#endif
