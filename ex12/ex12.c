#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("Too few args\n");
	} else if(argc > 1 && argc < 4) {
		printf("Your arguments: \n");

		for(int i = 0; i < argc; i++) {
			printf("%s ", argv[i]);
		}
		printf("\n");
	} else {
		printf("Too many args\n");
	}

	return 0;
}