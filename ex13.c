#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Expected at least one argument\n");
		return 1;
	}

	for(int i = 0; argv[1][i] != '\0'; i++) {
		char letter = argv[1][i];

		switch(letter) {
			case 'a':
			case 'A':
				printf("%d: 'A'\n", i);
				break;
			case 'e':
			case 'E':
				printf("%d: 'E'\n", i);
				break;
			case 'i':
			case 'I':
				printf("%d: 'I'\n", i);
				break;
			case 'o':
			case 'O':
				printf("%d: 'O'\n", i);
				break;
			case 'u':
			case 'U':
				printf("%d: 'U'\n", i);
				break;
			case 'y':
			case 'Y':
				if(i > 2) {
					printf("%d: 'Y'\n", i);
				}
				break;
			default:
				printf("%d: %c is not a vowel\n", i, letter);
		}
	}

	int argi;
	int i;
	char letter;
	for(argi = 1; argi < argc; argi++) {
		printf("Analizing argument %d\n", argi);

		for(i = 0, letter = argv[argi][i]; letter != '\0'; i++, letter = argv[argi][i]) {
			char canonic_letter = letter;
			if(letter >= 'A' && letter <= 'Z') {
				canonic_letter -= ('A' - 'a');
			}

			switch(canonic_letter) {
				case 'a':
					printf("%d: 'A'\n", i);
					break;
				case 'e':
					printf("%d: 'E'\n", i);
					break;
				case 'i':
					printf("%d: 'I'\n", i);
					break;
				case 'o':
					printf("%d: 'O'\n", i);
					break;
				case 'u':
					printf("%d: 'U'\n", i);
					break;
				case 'y':
					if(i > 2) {
						printf("%d: 'Y'\n", i);
					}
					break;
				default:
					printf("%d: %c is not a vowel\n", i, letter);
			}
		}
	}

	return 0;
}