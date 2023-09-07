// C program to set the value
// of unsigned char array during runtime

#include <stdio.h>
#include <string.h>

int main()
{

	// Initial unsigned char array
	unsigned char arr[3];

	// Print the initial array
	printf("Initial unsigned char array:\n");
	for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
		printf("%c ", arr[i]);
	}
	printf("\n");

	printf("\n");
	return 0;
}
