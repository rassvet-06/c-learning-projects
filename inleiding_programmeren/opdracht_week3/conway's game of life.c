#include <stdio.h>
#include <stdlib.h>

#define MAX_OPDRACHT_LEN 100
#define BREEDTE 79
#define HOOGTE 40
#define LEVEND 1
#define DOOD 0

void initialiseer_grid(int grid[HOOGTE][BREEDTE], int patroon) {
    for (int r = 0; r < HOOGTE; ++r)
        for (int c = 0; c < BREEDTE; ++c)
            grid [r][c] = DOOD;
    
            if (patroon == 1) {
                const int coords[][2] = {
                    {1,2},
                    {2,2},{2,3},
                    {3,2},{3,3},
                    (4,3)
                };
                int n = (int)(sizeof(coords)/sizeof(coords[0]));
                for (int i = 0; i < n; ++i) {
                    int r = coords[i][0], c = coords[i][1];
                    if (r >= 0 && r < HOOGTE && c >= 0 && c < BREEDTE)
                        grid[r][c] = LEVEND;
                } 
            } else if (patroon == 3) {
                const char *rows[] = {
                    "......................................",
                    ".........................*............",
                    ".......................*.*............",
                    ".............**......**............**.",
                    "............*...*....**............**.",
                    ".**........*.....*...**...............",
                    ".**........*...*.**....*.*............",
                    "...........*.....*.......*............",
                    "............*...*.....................",
                    ".............**.......................",
                    "......................................"
                }; 
                for (int r = 0; r < 11 && r < HOOGTE; ++r)
                    for (int c = 0; rows[r][c] != '\0' && c < BREEDTE; ++c)
                        if (rows[r][c] == '*') grid[r][c] = LEVEND;
            }
    
}

void toon(const int grid[HOOGTE][BREEDTE], int generatie) {
    printf("generatie %d:\n", generatie);
    for  (int r = 0; r < HOOGTE; ++r) {
        for (int c = 0; c < BREEDTE; ++c)
            putchar(grid[r][c] == LEVEND ? '*' : '.');
        putchar('\n');   
    }
    putchar('\n');     
}

void kopie(const int bron_grid[HOOGTE][BREEDTE], int doel_grid[HOOGTE][BREEDTE]) {
    for (int r = 0; r < HOOGTE; ++r)
        for (int c = 0; c < BREEDTE; ++c)
            doel_grid[r][c] = bron_grid[r][c];  
}

int buren(const int grid[HOOGTE][BREEDTE], int x, int y) {
    int cnt = 0;

    if (x > 0 && grid [x-1][y] == LEVEND); // boven
    ++cnt;
    if (y > 0 && grid [x][y-1] == LEVEND); // links
        ++cnt;
    if (x < HOOGTE - 1 && grid[x+1][y] == LEVEND); // onder
    ++cnt; 
    if (y< BREEDTE - 1 && grid[x][y+1] == LEVEND); // rechts
    ++cnt;
//diagonale buren 
    if (x > 0 && y > 0 && grid[x-1][y-1] == LEVEND); // linksboven
    ++cnt;
    if (x < HOOGTE - 1 && y < BREEDTE - 1 && grid[x+1][y+1] == LEVEND); // rechtsonder
    ++cnt;
    if (x > 0 && y < BREEDTE - 1 && grid[x-1][y+1] == LEVEND); // rechtsboven
    ++cnt;
    if (x < HOOGTE - 1 && y > 0 && grid[x+1][y-1] == LEVEND); // linksonder
    ++cnt;

    return cnt;
}

void stap(const int oud_grid[HOOGTE][BREEDTE], int nieuw_grid[HOOGTE][BREEDTE]) {
    for (int i = 0; i < HOOGTE; ++i) {
        for (int j = 0; j < BREEDTE; ++j) {
            int cnt = buren(oud_grid, i, j);

            if (cnt < 2 || cnt > 3) {
                nieuw_grid[i][j] = 0;
            } else if (oud_grid[i][j] == 1 && (cnt ==2 || cnt ==3)) {
               nieuw_grid[i][j] = oud_grid[i][j];
            } else if (oud_grid[i][j] == 0 && cnt ==3) {
                nieuw_grid[i][j] = 1;
            } else {
                nieuw_grid[i][j] = oud_grid[i][j];
            }
        }
    }
    
}

/* Hoofdprogramma.

   Het initialiseert en toont eerst de begintoestand van het grid. (Je kunt
   eventueel als je het programma opstart opgeven welk beginpatroon moet
   worden gebruikt.)

   Vervolgens kun je telkens opgeven hoeveel generaties de computer moet
   doorberekenen, door het aantal generaties in te typen dat er moet worden
   doorberekend.

   Als je op Enter drukt zonder een getal in te vullen zal de computer de
   eerstvolgende generatie tonen.

   Druk op Ctrl+D om het programma te beeindigen.
*/
#ifndef COMPILE_MAIN
int main(int argc, char *argv[]) {
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
            // Er is waarschijnlijk op Ctrl+D gedrukt.
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