// main.c

#include "../inc/cstr/CSTRpredefines.h"

#if CSTR_DEV == 1
# 	include "../inc/cstr/cstr.h"
#else 
# 	include <cstr/cstr.h>
#endif // !defined(CSTR_DEV)

#include <stdio.h> 
#include <string.h>
#include <stdarg.h>

int main(void)
{
	printf("-= CSTR Testing =-\n");
	printf("TESTING 1\n");

	{
		CSTR str = cstr_init();

		if (cstr_set(&str, "Hello world!"))
			printf("Could not set str to its corresponding value\n");

		if (cstr_add(&str, " Goodbye world, this is a very large text to add") == CSTR_FORCECAP_LIMIT)
	    	printf("Addition failed due to forceCap limit\n");

		cstr_substr(&str, 3, 13);
		cstr_insert(&str, "something", 4);

		printf("-- SUBSTR DATA --\n");
		printf("Found str for `world` index pos: %lu\n", cstr_find(&str, "world"));
		printf("DATA: %s\n", str.data);
		printf("CAP: %lu\n", (unsigned long)str.cap);
		printf("LEN: %lu\n", (unsigned long)str.len);
		printf("ForceCap?: %s\n", cstr_bool(str.forceCap));

		cstr_clear(&str);

		printf("Cleared data: %s\n", str.data);

		cstr_destroy(&str);
	}

	printf("TESTING 2\n");

	{
		CSTR str = cstr_init();

		if (cstr_set(&str, "Hello world! How are you?"))
			printf("Could not set the str to its corresponding value\n");

		if (cstr_reverse(&str))
			printf("Error reversing the str values");

		printf("DATA: %s\n", str.data);

		cstr_destroy(&str);
	}

	printf("TESTING 3\n");

	{
#ifdef CSTR_ENABLE_GET_RETURN
		CSTR str = cstr_init();

		printf("%s\n", CSTR_GET_RETURN(str, cstr_set, "hey world").data);

		cstr_destroy(&str);
#else
		printf("Cannot do: TEST 3\n");
# 		warning "Cannot do: TEST 3"
#endif // defined(CSTR_ENABLE_GET_RETURN)
	}

	printf("TESTING 4\n");

	{
		printf("TEST 4.1\n");

		CSTR str = cstr_init();

		cstr_set(&str, "Hello world, how are you?");
		cstr_replace(&str, "world", "hell");

		printf("STR: %s\n", str.data);

		printf("TEST 4.2\n");

		cstr_set(&str, "Hello world! How's the world?");
		cstr_replaceAll(&str, "world", "earth");

		printf("STR: %s\n", str.data);

 		cstr_destroy(&str);
	}

	printf("TESTING 5\n");

	{
		CSTR str = cstr_init(); 	cstr_set(&str, "Hello world");
		CSTR str2 = cstr_init(); 	cstr_set(&str2, "Bye world");

		printf("Content of: str; %s\n", str.data);
		printf("Content of: str2; %s\n", str2.data);

		if (cstr_comp(str, str2)) printf("str and str2 are the same\n");
		else printf("str and str2 are the NOT same\n");

		cstr_destroy(&str);
		cstr_destroy(&str2);
	}

	printf("TESTING 6\n");

	{
		CSTR str = cstr_init();

		cstr_set(&str, "Hello world!");

		printf("TESTING 6.1\n");

		printf("DATA: %s\n", str.data);
		printf("Does it end with `rld!`? %s\n", cstr_bool(cstr_endsWith(str, "rld!")));
		printf("TESTING 6.2\n");
		printf("Does it start with `llo`? %s\n", cstr_bool(cstr_startsWith(str, "llo")));

		cstr_destroy(&str);
	}

	printf("TESTING 7\n");

	{
		CSTR str = cstr_init();

		cstr_set(&str, "Hello world! How are you my friend? Building `aquin.app` right now");

		size_t foundStr = cstr_find(&str, "How are you my friend?");

		cstr_erase(&str, foundStr, foundStr + strlen("How are you my friend?"));

		printf("DATA: %s\n", str.data);

		cstr_destroy(&str);
	}

	printf("TESTING 8\n");

	{
#ifdef CSTR_ENABLE_GET_CONST_RETURN
		CSTR str = cstr_init();

		cstr_set(&str, "Random data...");

		printf("TMP DATA: %s\n", CSTR_GET_CONST_RETURN(str, cstr_fromInt, 12345).data);
		printf("CURRENT DATA: %s\n", str.data);

		cstr_fromDouble(&str, 3.14159);

		printf("DATA: %s\n", str.data);

		cstr_destroy(&str);
#else
		printf("Cannot do TEST 8\n");
# 		warning "Cannot do TEST 8"
#endif // CSTR_ENABLE_GET_CONST_RETURN
	}

	printf("TESTING 9\n");

	{
		CSTR str = cstr_init();
		cstr_set(&str, "Hello world"); // NOTE: Comment OR Uncomment this line for more testing

		printf("Is empty? %s\n", cstr_bool(cstr_empty(&str)));

		cstr_destroy(&str);
	}

	{
		CSTR str = cstr_init();

		cstr_set(&str, "The quick brown fox jumps over the lazy dog");

		printf("Checking how many 'o's are there in; %s\n", str.data);

		int counts = cstr_countChar(&str, 'o');

		printf("There are: %i\n", counts);

		cstr_destroy(&str);
	}

	{
		CSTR str = cstr_init();

		cstr_set(&str, "Hey, how are you pcannon09? Oh, Hey, how are you Paul?");

		printf("Checking how many 'Hey's are there in; %s\n", str.data);

		int counts = cstr_count(&str, "Hey");

		printf("There are: %i\n", counts);

		cstr_destroy(&str);

	}

	return 0;
}

