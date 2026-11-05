/*

   Dit programma implementeert de Caesar en Vigenere geheimschriften, en code
   om cryptogrammen te kraken.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_GROOTTE 500000
#define MAX_SLEUTEL 16

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

/* Functie: versimpel
*  Versimpelt een string
*  Alleen letters worden behouden 
*  en omgezet naar kleine letters
*
*  str: pointer naar string die moet versimpeld worden
*/
void versimpel(char *str) {
    char *lees = str;
    char *schrijf = str;

    while (*lees) {
        unsigned char uc = (unsigned char)*lees;
        if (isalpha(uc)) {
            *schrijf++ = (char)tolower(uc);
        }
        lees++;
    }
    *schrijf = '\0';
}

/* Functie: caesar_versleutel
*  Voert codering volgens Caesar - cijfer uit
*  Alleen letters worden aangepast 
*  Andere tekens blijven ongewijzigd
* 
*  bericht: pointer naar de te versleutelen string
*/
void caesar_versleutel(char *bericht) {  
    char *p = bericht;
    while (*p) {
        unsigned char uc = (unsigned char)*p;
        int ascii_code = uc - 'a';
        int caesar_code = (ascii_code + 3) % 26;
        char caesar_char = (char)('a' + caesar_code);
        *p++ = caesar_char;
    }
}

/* Functie: opdracht_caesar_versleutel
*  Leest een gegeven bestand
*  Versimpelt de tekst
*  Voert Caesar versleuteling uit
*  Drukt het resultaat af
*
*  fn: de bestandsnaam
*/
void opdracht_caesar_versleutel(const char *fn) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    caesar_versleutel(bericht);
    printf("Caesar versleuteld: %s\n", bericht);
}

/* Functie: caesar_ontsleutel
*  Ontsleutelt bericht met Caesar cijfer 
*
*  bericht: pointer naar de te ontsleutelen string
*/
void caesar_ontsleutel(char *bericht) {
    char *p = bericht;
    while (*p) {
        unsigned char uc = (unsigned char)*p;
        int ascii_code = uc - 'a';
        int caesar_code = (ascii_code - 3) % 26;
        char caesar_char = (char)('a' + caesar_code);
        *p++ = caesar_char;
    }
}

/* Functie: opdracht_caesar_ontsleutel
*  Leest een gegeven bestand
*  Versimpelt de tekst
*  Voert Caesar ontsleuteling uit
*  Drukt het resultaat af
*
*  fn: de bestandsnaam
*/
void opdracht_caesar_ontsleutel(const char *fn) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht); // (Dit is misschien niet nodig)
    caesar_ontsleutel(bericht);
    printf("Caesar ontsleuteld: %s\n", bericht);
}

/* Functie: vigenere_versleutel
*  Versleutelt bericht met Vigenere cijfer
*  Alleen letters worden aangepast 
*  Andere tekens blijven ongewijzigd
* 
*  bericht: pointer naar de te versleutelen string
*  sleutel: pointer naar de sleutelstring
*/
void vigenere_versleutel(char *bericht, const char *sleutel) {
    int i = 0;
    int slen = (int)strlen(sleutel);
    while (bericht[i]) {
        unsigned char uc = (unsigned char)bericht[i];
        int ascii_code = uc - 'a';
        int sleutel_code = sleutel[i % slen] - 'a';
        int vigenere_code = (ascii_code + sleutel_code) % 26;
        char vigenere_char = (char)('a' + vigenere_code);
        bericht[i] = vigenere_char;
        i++;
    }
}

/* Functie: opdracht_vigenere_versleutel
*  Leest een gegeven bestand
*  Versimpelt de tekst
*  Voert Vigenere versleuteling uit
*  Drukt het resultaat af
*
*  fn: de bestandsnaam
*  sleutel: pointer naar de sleutelstring
*/
void opdracht_vigenere_versleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    vigenere_versleutel(bericht, sleutel);
    printf("Vigenere versleutel: %s\n", bericht);
}

/* Functie: vigenere_ontsleutel
*  Leest een gegeven bestand
*  Versimpelt de tekst
*  Voert Vigenere ontsleuteling uit
*  
*  bericht: pointer naar de te ontsleutelen string
*  sleutel: pointer naar de sleutel string
*/
void vigenere_ontsleutel(char *bericht, const char *sleutel) {
    int i = 0;
    int slen = (int)strlen(sleutel);
    while (bericht[i]) {
        unsigned char uc = (unsigned char)bericht[i];
        int vigenere_code = uc - 'a';
        int sleutel_code = sleutel[i % slen] - 'a';
        int ascii_code = (vigenere_code - sleutel_code + 26) % 26;
        char bericht_char = (char)('a' + ascii_code);
        bericht[i] = bericht_char;
        i++;
    }
}

/* Functie: opdracht_viginere_ontsleutel
*  Leest een gegeven bestand 
*  Versimpelt de tekst 
*  Voert Vigenere ontsleuting uit 
*  Drukt resultaat af 
*  
*  fn: de bestandnaam 
*  sleutel: pointer naar een sleutel string 
*/
void opdracht_vigenere_ontsleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht); 
    vigenere_ontsleutel(bericht, sleutel);
    printf("Vigenere ontsleutel: %s\n", bericht);
}

/* Functie: bepaal_frequenties
*  Leest een gegeven bestand
*  Bepaalt frequentie van elke letter in een gegeven tekst 
*  Telt hoe vaak elke letter voorkomt in een array 
*  Frequenties krijgen we door te delen door totaal aantal letters
*  
*  tekst: pointer naar string die wij moeten analyseren 
*  p: pointer naar een array waar wij frequenties opslaan
*/
void bepaal_frequenties(const char *tekst, double *p) {
    int freq[26] = {0};
    const char *lees = tekst;
    double totaal = 0;

    while (*lees) {
        unsigned char uc = (unsigned char)*lees;
        int index = uc - 'a';
        freq[index]++;
        totaal++;
        lees++;
    }
    for (int i = 0; i < 26; i++) {
        p[i] = (double)freq[i] / totaal;
    }
}

/* Functie: opdracht_frequenties
*  Leest een gegeven bestand
*  Versimpelt de tekst
*  Bepaalt frequenties van letters in de tekst
*  Drukt de frequenties af
*
*  fn: de bestandsnaam
*/
void opdracht_frequenties(const char *fn) {
    char tekst[MAX_GROOTTE];
    double p[26]; 
    lees_bestand(fn, tekst);
    versimpel(tekst); 
    bepaal_frequenties(tekst, p);
    // printf("Frequenties: %s\n", tekst);
    for (int i = 0; i < 26; i++) {
        char letter = (char)('a' + i);
        printf("%c: %0.3f\n", letter, p[i]);
    }
}

/* Functie: gelijkenis
*  Berekent de gelijkenis tussen twee frequentie arrays 
*  Gelijkenis berekenen door middel van inproduct
*  
*  p: pointer naar de eerste frequentie array
*  q: pointer naar de tweede frequentie array
*/
double gelijkenis(const double *p, const double *q) {
    double som = 0.0;
    for (int i = 0; i < 26; i++) {
        som += p[i] * q[i];
    }
    return som;
}

/* Functie: opdracht_vergelijk
*  Leest twee gegeven bestanden
*  Versimpelt de teksten
*  Bepaalt frequenties van letters in beide teksten
*  Berekent de gelijkenis tussen de twee frequentie arrays
*  Drukt de gelijkenis af
*
*  fn_tekst1: de bestandsnaam van de eerste tekst
*  fn_tekst2: de bestandsnaam van de tweede tekst
*/
void opdracht_vergelijk(const char *fn_tekst1, const char *fn_tekst2) {
    char tekst1[MAX_GROOTTE];
    char tekst2[MAX_GROOTTE];
    double p[26];
    double q[26];
    double score_gelijkenis;

    lees_bestand(fn_tekst1, tekst1);
    lees_bestand(fn_tekst2, tekst2);
    versimpel(tekst1);
    versimpel(tekst2);
    bepaal_frequenties(tekst1, p);
    bepaal_frequenties(tekst2, q);
    score_gelijkenis = gelijkenis(p, q);
    printf("Gelijkenis is %0.3f\n", score_gelijkenis);
}

/* Functie: zoek_sleutel
*  Zoekt de beste sleutel voor een gegeven cijfertekst
*  Probeer elke letter voor elke positie in de sleutel
*  Houd de beste letter voor elke positie bij
*  Berekent de score voor de gevonden sleutel
*  
*  cijfertekst: pointer naar de te kraken cijfertekst
*  sleutel: pointer naar een array waar wij de gevonden sleutel opslaan
*  slen: de lengte van de sleutel
*  frequenties_taal: pointer naar een array 
*/
double zoek_sleutel(const char *cijfertekst,
                    char *sleutel,
                    size_t slen,
                    const double *frequenties_taal) {

    size_t tlen = strlen(cijfertekst);
    char bericht[tlen + 1];
    const char *origineel = cijfertekst;

    /* Sleutel is een string van lengte slen */
    for (size_t i = 0; i < slen; ++i) {
        sleutel[i] = 'a';
    }
    sleutel[slen] = '\0';

    double frequenties_text[26];

    /* Voor elke positie in de sleutel, probeer elke letter en houd de beste bij */
    for (size_t pos = 0; pos < slen; ++pos) {
        double beste_score_pos = -INFINITY;
        char best_char = 'a';

        for (char c = 'a'; c <= 'z'; ++c) {
            sleutel[pos] = c;

            /* Score voor deze letter op deze positie */
            strcpy(bericht, origineel);
            vigenere_ontsleutel(bericht, sleutel);
            bepaal_frequenties(bericht, frequenties_text);
            double score = gelijkenis(frequenties_text, frequenties_taal);

            if (score > beste_score_pos) {
                beste_score_pos = score;
                best_char = c;
            }
        }

        /* Beste letter voor deze positie in de sleutel */
        sleutel[pos] = best_char;
    }

    /* Bereken de uiteindelijke score voor de gevonden sleutel */
    strcpy(bericht, origineel);
    vigenere_ontsleutel(bericht, sleutel);
    bepaal_frequenties(bericht, frequenties_text);
    double final_score = gelijkenis(frequenties_text, frequenties_taal);
    return final_score;
}

/* Functie: opdracht_kraak
*  Leest een gegeven cijfertekst en een taalvoorbeeld
*  Versimpelt beide teksten
*  Bepaalt frequenties van letters in de taalvoorbeeld
*  Probeert sleutels van lengte 1 tot MAX_SLEUTEL
*  Voor elke lengte, zoekt de beste sleutel
*  Houd de beste sleutel en het bijbehorende bericht bij
*  Drukt de beste sleutel en het bijbehorende bericht af
*
*  fn_cijfertekst: de bestandsnaam van de cijfertekst
*  fn_taal: de bestandsnaam van het taalvoorbeeld
*/
void opdracht_kraak(const char *fn_cijfertekst, const char *fn_taal) {
    /* Lees cijfertekst en taalvoorbeeld uit bestanden, probeer sleutels */
    char bericht[MAX_GROOTTE];
    lees_bestand(fn_cijfertekst, bericht);
    versimpel(bericht);

    char taal[MAX_GROOTTE];
    lees_bestand(fn_taal, taal);
    versimpel(taal);

    double frequenties_taal[26];
    bepaal_frequenties(taal, frequenties_taal);

    size_t slen;
    double beste_score = -INFINITY;
    char beste_sleutel[MAX_SLEUTEL + 1] = {0};
    char beste_bericht[MAX_GROOTTE] = {0};

    for (slen = 1; slen <= MAX_SLEUTEL; slen++) {
        /* Maak ruimte voor de terminator */
        char sleutel[slen + 1];

        double score = zoek_sleutel(bericht, sleutel, slen, frequenties_taal);
        printf("Sleutel %s: score=%lf\n", sleutel, score);

        if (score > beste_score) {
            beste_score = score;
            strcpy(beste_sleutel, sleutel);
            /* Ontsleutel bericht met de beste sleutel */
            char temp[MAX_GROOTTE];
            strcpy(temp, bericht);
            vigenere_ontsleutel(temp, sleutel);
            strcpy(beste_bericht, temp);
        }
    }

    printf("sleutel: %s\n", beste_sleutel);
    printf("bericht: %s\n", beste_bericht);
}

/* Drukt gebruiksinformatie af 
*  en beeindigt het programma met een foutcode
*  stderr: standaard foutuitvoer
 */
void gebruik(void) {
    fprintf(stderr,
            "gebruik: vigenere caesar_versleutel <bericht>\n"
            "         vigenere caesar_ontsleutel <bericht>\n"
            "         vigenere versleutel <bericht> <sleutel>\n"
            "         vigenere ontsleutel <cijfertekst> <sleutel>\n"
            "         vigenere frequenties <tekst>\n"
            "         vigenere vergelijk <tekst1> <tekst2>\n"
            "         vigenere kraak <cijfertekst> <voorbeeld>\n");
}

/* Hoofdfunctie
*/
#ifndef COMPILE_MAIN
int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        gebruik();
        return EXIT_FAILURE;
    }

    const char *opdr = argv[1];
    if (strcmp(opdr, "caesar_versleutel") == 0 && argc == 3) {
        opdracht_caesar_versleutel(argv[2]);
    } else if (strcmp(opdr, "caesar_ontsleutel") == 0 && argc == 3) {
        opdracht_caesar_ontsleutel(argv[2]);
    } else if (strcmp(opdr, "versleutel") == 0 && argc == 4) {
        opdracht_vigenere_versleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "ontsleutel") == 0 && argc == 4) {
        opdracht_vigenere_ontsleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "frequenties") == 0 && argc == 3) {
        opdracht_frequenties(argv[2]);
    } else if (strcmp(opdr, "vergelijk") == 0 && argc == 4) {
        opdracht_vergelijk(argv[2], argv[3]);
    } else if (strcmp(opdr, "kraak") == 0 && argc == 4) {
        opdracht_kraak(argv[2], argv[3]);
    } else {
        gebruik();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#endif
