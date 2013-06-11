#include <stdio.h>

void printPointed(int count, char **names, int *ages)
{
	int i = 0;
	for(i = 0; i < count; i++) {
		printf("%s -> %d\n", names[i], ages[i]);
	}
}

int main(int argc, char *argv[])
{
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank", "Mary",
		"John", "Lisa"
	};

	int count = sizeof(ages) / sizeof(int);
	int i = 0;

	for(i = 0; i < count; i++) {
		printf("%s has %d years alive.\n", names[i], ages[i]);
	}

	printf("---\n");

	int *cur_age = ages;
	char **cur_name = names;

	for(i = 0; i < count; i++) {
		printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
	}

	printf("---\n");

	for(i = 0; i < count; i++) {
		printf("%s is %d years old.\n", cur_name[i], cur_age[i]);
	}

	printf("---\n");

	for(cur_name = names, cur_age = ages;
		(cur_age - ages) < count;
		cur_name++, cur_age++) {

		printf("%s lived %d years so far.\n", *cur_name, *cur_age);
	}

	char *name = cur_name[0];
	printf("xxx -> %s\n", name);

	cur_age = ages;
	cur_name = names;

	for(i = 0; i < count; i++) {
		printf("%s points to > %p\n", cur_name[i], &cur_name[i]);
	}

	printPointed(count, names, ages);
	printPointed(count, cur_name, cur_age);
	printPointed(count, &names[0], &ages[0]);
}