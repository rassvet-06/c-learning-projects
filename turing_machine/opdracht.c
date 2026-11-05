#include <stdio.h>
#include <math.h> 

int main () {
    int getal;
    getal = scanf("%d");
    if (getal % 2 == 0) {
        printf("%dEVEN", getal); // even
    } 
    else if (getal % 2 != 0) {
        printf("%dODD", getal); // odd
    }
    return 0;
}


