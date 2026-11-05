

#define MAX_GROOTTE 10000

/* Functie: lees_bestand 
*  Leest inhoud van een tekstbestand in een array
* 
*  fn: bestandsnaam
*  str: pointer naar een array om inhoud op te slaan
*/
void lees_bestand(const char *fn, char *str) {
    FILE *fh = fopen(fn, "r");
    if (fh == NULL) {
        perror("Kan invoer niet lezen");
        exit(EXIT_FAILURE);
    }
    size_t grootte = fread(str, 1, MAX_GROOTTE - 1, fh);
    fclose(fh);
    if (grootte == MAX_GROOTTE - 1) {
        fprintf(stderr, "De invoer was te lang.\n");
        exit(EXIT_FAILURE);
    }
    str[grootte] = '\0';
}