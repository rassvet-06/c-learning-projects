
#include <stdio.h>


int main (void) {
    int guess;
    int guessed;

    guessed = 0;
    int lower_boundary = 0;
    int upper_boundary = 100;
    int space_up; // space between guess and upper boundary 
    int space_down; // space between guess and lower boundary

    while (guessed == 0) {

        printf("Doe een gok:\n");
        int code = scanf("%d", &guess);
        if (code != 1) {
            printf("Het lezen van een getal is niet gelukt. Ik stop ermee.\n");
            return 0;
        }

        if  (guess < 0  || guess > 100) {
            printf("Wrong. The number has to be between 0 and 100. \n");
            continue;
        }

        space_up = upper_boundary - guess;
        space_down = guess - lower_boundary;

        if (space_up == 1 && space_down == 1) {
            printf("Je hebt het geraden! \n");
            break;
        }

        if (space_down < space_up) {
            printf("Te laag.\n\n");
            if (space_down > 0) {
                lower_boundary = guess;
            }
            // printf("lower_boundary = %i\n", lower_boundary);
        } else {    // if space_down >= space_up
            printf("Te hoog.\n\n"); 
            if (space_up > 0) {
                upper_boundary = guess;
            }
            // printf("upper_boundary = %i\n", upper_boundary);

        }
    }

    return 0; 
}
