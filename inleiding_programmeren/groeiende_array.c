#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int *maak_array_groter(int *a, int *n1, int n2) {
    assert(n2 >= *n1);
    
    int *tmp = malloc(n2 * sizeof(int));
    memcpy(tmp, a, (*n1) * sizeof(int));
    free(a);

    *n1 += 1000;
    printf("n1 in the function: %d\n", *n1);

    return tmp;
}

int main() {
    int *myArray = malloc(10 * sizeof(int));
    int n1 = 10;
    int n2 = 20;
    myArray = maak_array_groter(myArray, &n1, n2);  // Pass &n1 by reference
    free(myArray);

    printf("n1 in the main function is: %d\n", n1);

    return 0;
}