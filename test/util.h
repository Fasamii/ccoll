#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../colors.h"

#define section(name) fprintf(stdout, "\n%s\n", name);

#define assert_eq(CONDITION, msg, ...)                                         \
	do {                                                                     \
		if (!(CONDITION)) {                                                \
			fprintf(                                                     \
			    stderr,                                                  \
			    "\n" YEL "Assert_eq" NOCOL "::" RED "Failed" NOCOL       \
			    "::" BLU "%s" NOCOL ":" BLU "%d" NOCOL "\n",             \
			    __FILE__, __LINE__                                       \
			);                                                           \
			fprintf(stderr, BLU "E>" NOCOL " ");                         \
			fprintf(stderr, msg, ##__VA_ARGS__);                         \
			fprintf(stderr, "\n\n");                                     \
                                                                               \
			exit(1);                                                     \
		} else {                                                           \
			fprintf(                                                     \
			    stdout,                                                  \
			    YEL "Assert_eq" NOCOL "::" GRN "Success" NOCOL "::" BLU  \
				  "%d" NOCOL ":",                                      \
			    __LINE__                                                 \
			);                                                           \
			fprintf(stdout, msg, ##__VA_ARGS__);                         \
			fprintf(stdout, "\n");                                       \
		}                                                                  \
	} while (0)

#define log(msg, ...)                                                          \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    BLU "Log" NOCOL "::" BLU "%s" NOCOL ":" BLU "%d" NOCOL ": ",   \
		    __FILE__, __LINE__                                             \
		);                                                                 \
		fprintf(stdout, msg, ##__VA_ARGS__);                               \
		fprintf(stdout, "\n");                                             \
	} while (0)

char random_lowercase() { return 'a' + rand() % 26; }
char random_uppercase() { return 'A' + rand() % 26; }
char random_digit() { return '0' + rand() % 10; }
char random_alphanumeric() {
	int choice = rand() % 3;
	switch (choice) {
	case 0: return random_lowercase();
	case 1: return random_uppercase();
	case 2: return random_digit();
	default: return random_lowercase();
	}
}
void generate_random_string(char *str, size_t length) {
	srandom(time(NULL));
	for (size_t i = 0; i < length; i++) {
		str[i] = random_alphanumeric();
	}
	str[length] = '\0';
}
