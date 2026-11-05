
#include <stdio.h>
#include <math.h> 

int main() // the main function 
{
	int a, b, x, y, code;

	printf("Geef de waarde voor a: ");
	code = scanf("%d", &a);
	// 
	if (code != 1) {
		printf("Het lezen van een getal is niet gelukt. Ik stop ermee.\n");
		return 0;
	}

	printf("Geef de waarde voor b: ");
	code = scanf("%d", &b);
	
	if (code != 1) {
		printf("Het lezen van een getal is niet gelukt. Ik stop ermee.\n");
		return 0;
	}
    // printf("a = %d\n", a);
	// printf("b = %d\n", b);

	// a= x + y en b= x-  y; x= a - y en x = b + y 
	y = (a - b) / 2;
	
	x = a - y;
	printf("x = %d\n", x);
	printf("y = %d\n", y);

	return 0;

}
