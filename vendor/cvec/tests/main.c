#include "../inc/cvec/cvec.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// main() function helpers go here and beyond

void printElements(CVEC vec)
{
	for (size_t i = 0 ; i < vec.size ; i++)
	{
		char **result = cvec_get(&vec, i);

		if (!result)
			break;

		printf("ELEMENT %lu: %s\n", i, *result);
	}
}

int main(void)
{
	printf("-= VECTOR ELEMENTS TESTING =-\n");
	printf("-----\n");

	CVEC vec = cvec_init(-1, sizeof(char*));

	cvec_push(&vec, char*, ",,,,");
	cvec_push(&vec, char*, "Hello");
	cvec_push(&vec, char*, "World");
	cvec_push(&vec, char*, ".");
	cvec_push(&vec, char*, "It is me, pcannon09");

	printf("Current elements:\n");

	for (unsigned int i = 0 ; i < vec.size ; i++)
	{
		char **result = cvec_get(&vec, i);

		if (!result) continue;

		printf("ELEMENT %d: %s\n", i, *result);
	}

	printf("Removing First Found `Hello`\n"); cvec_delIndex(&vec, char*, "Hello", 0);
	printf("Removing All `Hello`\n"); cvec_del(&vec, char*, "Hello");

	printf("Current elements:\n");

	printElements(vec);

	printf("Found index for `World`: ");
	printf("%d\n", cvec_find(&vec, char*, "World"));

	printf("Reversed vector:\n");

	cvec_reverse(&vec);

	printElements(vec);

	printf("Reversed vector (AGAIN):\n");

	cvec_reverse(&vec);

	printElements(vec);

	printf("Finding `World` and setting that exact index to `Earth`...\n");

	cvec_set(
			&vec,
			char*,
			"Earth",
			cvec_find(&vec, char*, "World")
			);

	for (size_t i = 0 ; i < vec.size ; i++)
	{
		char **result = cvec_get(&vec, i);

		if (!result)
			break;

		printf("ELEMENT %lu: %s\n", i, *result);
	}

	printf("Removing index 0 and then setting index 1 to an empty str...\n");

	cvec_remove(&vec, 0);
	cvec_set(&vec, char*, "", 1);

	printElements(vec);

	printf("Removing the last element\n");

	cvec_popBack(&vec);
	printElements(vec);

	printf("Setting the 1st element to index 0 with cvec_pushFront() function\n");

	cvec_pushFront(&vec, char*, "1st element");
	printElements(vec);

	printf("Adding `Bye` to element 2\n");

	cvec_pushIndex(&vec, char*, 2, "Bye");
	printElements(vec);

	printf("Size of the vector: %lu\n", vec.size);
	printf("Size of the vector capacity: %lu\n", vec.cap);

	{
		int pos = 9;

		printf("1) Is size of `%d` inside the bounds of vector capacity? %s\n", pos, cvec_atCap(&vec, pos) ? "yes" : "no");
	}

	{
		int pos = 1;

		printf("2) Is size of `%d` inside the bounds of vector? %s\n", pos, cvec_at(&vec, pos) ? "yes" : "no");
	}

	CVEC vecMerge = cvec_init(3, sizeof(char*));

	vecMerge.dynamicCap = false;

	cvec_push(&vecMerge, char*, "Hey");
	cvec_push(&vecMerge, char*, "this");
	cvec_push(&vecMerge, char*, "is some...");
	cvec_push(&vecMerge, char*, "text");
	cvec_push(&vecMerge, char*, "As you can see, you can set the size in cvec_init() function and setting an option to false");
	cvec_push(&vecMerge, char*, "So you won't see this in the program itself");

	printf("Merging vecMerge to vec\n");
	printf("Content with vecMerge\n");

	printElements(vecMerge);

	cvec_merge(&vec, &vecMerge);

	printf("Merged:\n");
	printElements(vec);

	printf("Splitting string with comas (,)\n");

	char *message = "Hello world, how are you, my friend?";

	CVEC tokens = cvec_init(0, sizeof(char*));
	cvec_split(&tokens, message, ",");

	printElements(tokens);

	printf("Splitting string with spaces ( )\n");
	char *message2 = "Hey, how are you?";

	cvec_split(&tokens, message2, " ");

	printElements(tokens);

	/* END */

	cvec_destroy(&vec);
	cvec_destroy(&vecMerge);
	cvec_destroy(&tokens);

	return 0;
}

