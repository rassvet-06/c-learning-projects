
#include <stdio.h>
#include <math.h> 

int main(void) // the main function 

{
    int lines = 21; //the amount of lines total 

    int line_number = 0;
    int max_points_in_line;
    int max_spaces_in_line; 

    // Upper part with increasing number of lines
    while (line_number < 14) {

        // Print current line

        // Print spaces
        int spaces_in_line = 0;
        max_spaces_in_line = line_number * 2;
        while (spaces_in_line < max_spaces_in_line) {
            printf(" ");
            spaces_in_line++;
        }

        // Print rest of the line
        printf("*");
        max_points_in_line = line_number * 2; 
        int points_in_line = 0;
        while(points_in_line < max_points_in_line) {
            printf("..");
            points_in_line += 2;
        }
        printf("*\n");

        line_number++;
        // Finished printing current line
    }

    // Lower part with decreasing number of points
    max_points_in_line = 23;
    while (line_number <= lines) {

        // Print spaces
        int spaces_in_line = 0;
        max_spaces_in_line = line_number * 2;
        while (spaces_in_line < max_spaces_in_line) {
            printf(" ");
            spaces_in_line++;
        }

        // Print current line
        printf("*");
        int points_in_line = 0;
        while(points_in_line < max_points_in_line) {
            printf(".");
            points_in_line++;
        }
        printf("*\n");
        max_points_in_line -= 3;
        // Finished printing current line

        line_number++;  
    }

    // Print spaces
    int spaces_in_line = 0;
    max_spaces_in_line = line_number * 2;
    while (spaces_in_line < max_spaces_in_line) {
        printf(" ");
        spaces_in_line++;
    }

    printf("*\n");

    return 0;
}
