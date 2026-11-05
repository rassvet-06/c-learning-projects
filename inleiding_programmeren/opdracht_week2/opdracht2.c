/*

   Dit programma bevat verschillende functies waarmee de Collatz-reeks
   onderzocht kan worden.
*/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* berekenen van opvolgende waarde van getal in de collatz_reeks 
* eerst bepaal je als getal kleiner of gelijk is dan nul, anders als getal even is moet je getal door 2 delen
* en als getal oneven is moet je de getal eerts * 3 en dan + 1
* Uitvoer:
* altijd -1 als getal kleiner of gelijk is aan 0 
* anders getals moet of door 2 gedeeld worden of eerst keer 3 en dan + 1 
*/
int collatz_opvolger(int getal) {
    if (getal <= 0) {
        return -1;
    }
    else if (getal % 2 == 0) {
        getal = getal / 2;
    } 
    else if (getal % 2 != 0) {
        getal = 3 * getal + 1;
    }
    return getal;
}

/* collatz_stoptijd telt hoe veel stappen moet je nemen (hoe lang het duurt) voordat reeks bij 1 is
*  om bij antwoord te komen moet je bij elke reeks die je maakt stoptijd 1 groter maken (zo kan je tellen)
* als uitvoer krijg je dus wat de stoptijd (eerste commentair gerel) is 
*/
int collatz_stoptijd(int reeks) {
    int stoptijd = 1;
    while (reeks > 1) {
        if (reeks == 0) {
            reeks = -1;
        } 
        else if (reeks % 2 == 0) {
            reeks = reeks / 2;
        } 
        else if (reeks % 2 != 0) {
            reeks = 3 * reeks + 1;
        }
        stoptijd++;
    }
    return stoptijd;
}

/* deze functie zoekt dus de grootste getal dat voorkomt in de reeks 
* eerst maken wij een variabele maximun, dan moeten wij nog steeds de reeks berekenen
* tijdens reeks berekenen merken wij dat 1-2-4 cyclus gaat oneindig door en daarom moeten wij zelf grens instellen
* zodat de loop stopt en de reeks kan ook voor getallen na 1 berekend worden 
* wij weten dat de maximum in cyclus 1-2-4 altijd 4 is volgens de formule 
* dan vergelijk je elke waarde van de reeks 1 voor 1 tot dat je hebt alle getallen door genomen 
* dus: uitvoer maximum 
*/
int collatz_maximum(int reeks) {
    int maximum = reeks;
    while (reeks >= 1) {
        if (reeks <= 0) {
            return -1;
        }
        else if (reeks == 1) {
            if (maximum < 4) {
                maximum = 4;
            }
            return maximum;
        }
        else if (reeks % 2 == 0) {
            reeks = reeks / 2;
        } 
        else if (reeks % 2 != 0) {
            reeks = 3 * reeks + 1;
        }
        if (reeks > maximum) {
            maximum = reeks;
        }
    }
    return maximum;
}
/* functie bepaalt de hele reeks 
* als uitvoer krijg je elke waarde van de reeks als jij die functie aanroept 
* of je kan ook dezelfde code schrijven voor berekenen van reeks in de andere functie als die nodig is 
* als uitvoer krijg je hele reeks met een comma achter en een spatie 
*/
void collatz_toon_reeks(int reeks) {
    
    printf("%d, ", reeks);
    while (reeks > 1) {
        if (reeks == 0) {
            reeks = -1;
        } 
        else if (reeks % 2 == 0) {
            reeks = reeks / 2;
        } 
        else if (reeks % 2 != 0) {
            reeks = 3 * reeks + 1;
        }
        printf("%d", reeks);

        if (reeks > 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void collatz_info(int reeks) {
    printf("Reeks: ");
    collatz_toon_reeks(reeks);
    printf("Stoptijd: %d\n", collatz_stoptijd(reeks));
    printf("Maximum: %d\n", collatz_maximum(reeks));
}

//geeft dan het begingetal terug van de reeks met de grootste stoptijd
// eerst bereken je reeks 
// dan bereken en vergelijk stoptijd 
// als uitvoer krijg je de begin getal van de reeks met de kangste stoptijd
int collatz_zoek_langste(int a, int b) {  
    int reeks_voor = a;
    int langste_stoptijd = 0;
    int reeks_for_langste_stoptijd = -1;
    while (reeks_voor <= b) {
        int stoptijd_nu = 1;
        int reeks_nu = reeks_voor; 
        while (reeks_nu > 1) {
            if (reeks_nu == 0) {
                reeks_nu = -1;
            } 
            else if (reeks_nu % 2 == 0) {
                reeks_nu = reeks_nu / 2;
            } 
            else if (reeks_nu % 2 != 0) {
                reeks_nu = 3 * reeks_nu + 1;
            }
            stoptijd_nu++;
        }
        if (stoptijd_nu > langste_stoptijd) {
            langste_stoptijd = stoptijd_nu;
            reeks_for_langste_stoptijd = reeks_voor;
        }
        reeks_voor++;
    }
    return reeks_for_langste_stoptijd;
}
// functie geeft het begin getal terug van de reeks met de grootste maximum
int collatz_zoek_grootste(int a, int b) {
    int reeks_voor = a;
    int grootste_waarde = 0;
    int reeks_for_grootste_waarde = -1;
    while (reeks_voor <= b) {
        int reeks_nu = reeks_voor; 
        while (reeks_nu > 1) {
            if (reeks_nu == 0) {
                reeks_nu = -1;
            } 
            else if (reeks_nu % 2 == 0) {
                reeks_nu = reeks_nu / 2;
            } 
            else if (reeks_nu % 2 != 0) {
                reeks_nu = 3 * reeks_nu + 1;
            }
            if (reeks_nu > grootste_waarde) {
                grootste_waarde = reeks_nu;
                reeks_for_grootste_waarde = reeks_voor;
            }
        }
        reeks_voor++;
    }
    return reeks_for_grootste_waarde;
}

// onderzoek of onze geschreven functies kloppen
void collatz_onderzoek(int a, int b) {
    printf("Langste Collatz reeks tussen %d en %d:\n", a, b);
    int reeks_for_langste_stoptijd = collatz_zoek_langste(a, b);
    collatz_info(reeks_for_langste_stoptijd);

    printf("\n");

    printf("Collatz reeks met de grootste waarde tussen %d en %d:\n", a, b);
    int reeks_for_grootste_waarde = collatz_zoek_grootste(a, b);
    collatz_info(reeks_for_grootste_waarde);
}

// doet een overflow check (INT_MAX dus als het te groot voor int is)
// overflow kan nooit bij n/2 zijn 
// dus wij pakken de formule 3n+1 om te gaan uitrekenen 
void collatz_incorrect(void) {
    int reeks_min = 1, reeks_max = 1000000, reeks_voor = reeks_min;
    while (reeks_voor <= reeks_max) {
        int reeks_nu = reeks_voor; 
        while (reeks_nu > 1) {
            if (reeks_nu == 0) {
                printf("%d\n", reeks_voor);
                break;
            } 
            else if (reeks_nu % 2 == 0) {
                reeks_nu = reeks_nu / 2;
            } 
            else if (reeks_nu % 2 != 0) {
                if (reeks_nu >= (INT_MAX - 1) / 3) {
                    printf("%d\n", reeks_voor);
                    break;
                }
                reeks_nu = 3 * reeks_nu + 1;
            }
        }
        reeks_voor++;
    }
    printf("\n");
}

#ifndef COMPILE_MAIN
int main(int argc, char **argv) {

    if (argc == 1) {
        // Programma is aangeroepen zonder argumenten
        collatz_incorrect();
    } else if (argc == 2) {
        // Programma is aangeroepen met 1 argument.
        int reeks = atoi(argv[1]);
        // printf("reeks = %d\n", reeks);
        collatz_info(reeks);
    } else if (argc == 3) {
        // Programma is aangeroepen met 2 argumenten.
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        collatz_onderzoek(a, b);
    } else {
        printf("gebruik: opdracht2 [getal [tweede getal]]\n");
        return 1;
    }

    return 0;
}
#endif
