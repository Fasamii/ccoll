#if CCOLL_DEBUG

#include <stdio.h>

#include "./colors.h"

#define CCOLL_LOG(msg, ...)                                            \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    BLU "┌[ " RED "!" BLU " ]" BLU " LOG::[" NOCOL "Vec" BLU   \
			  "]:[" MAG "FOO " NOCOL "%s()" BLU "]:[" MAG "FILE " NOCOL  \
			  "%s" BLU "]:[" MAG "LINE " NOCOL "%d" BLU "]" NOCOL "\n",  \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, BLU "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)

#define CCOLL_WARN(msg, ...)                                           \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    YEL "┌[ " RED "!!" YEL " ]" BLU " WARN"                        \
			  "::[" NOCOL "Vec" BLU "]:[" MAG "FOO " NOCOL           \
			  "%s()" BLU "]:[" MAG "FILE " NOCOL "%s" BLU "]:[" MAG      \
			  "LINE " NOCOL "%d" BLU "]" NOCOL "\n",                     \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, YEL "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)

#define CCOLL_ERROR(msg, ...)                                          \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    RED "┌[ " RED "!!!" RED " ]" BLU " ERROR::[" NOCOL             \
			  "Vec" BLU "]:[" MAG "FOO " NOCOL "%s()" BLU "]:[" MAG  \
			  "FILE " NOCOL "%s" BLU "]:[" MAG "LINE " NOCOL "%d" BLU    \
			  "]" NOCOL "\n",                                            \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, RED "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)

#define CCOLL_ASSERT(CONDITION, msg, ...)                              \
	do {                                                                     \
		if (!(CONDITION)) {                                                \
			fprintf(                                                     \
			    stderr,                                                  \
			    RED "┌[///// [ASSERT FAIL] /////]" BLU "::[" NOCOL        \
				  "Vec" BLU "]:[" MAG "FOO " NOCOL "%s" BLU        \
				  "]:[" MAG "FILE " NOCOL "%s" BLU "]:[" MAG           \
				  "LINE " NOCOL "%d" BLU "]" NOCOL "\n",               \
			    __func__, __FILE__, __LINE__                             \
			);                                                           \
			fprintf(                                                     \
			    stderr, RED "└[msg]: " NOCOL msg NOCOL "\n\n",           \
			    ##__VA_ARGS__                                            \
			);                                                           \
		} else {                                                           \
			fprintf(                                                     \
			    stdout,                                                  \
			    GRN "┌[ASSERT SUCCESS]" BLU "::[" NOCOL "Vec" BLU    \
				  "]:[" MAG "FOO " NOCOL "%s" BLU "]:[" MAG          \
				  "FILE " NOCOL "%s" BLU "]:[" MAG "LINE " NOCOL      \
				  "%d" BLU "]" NOCOL "\n",                             \
			    __func__, __FILE__, __LINE__                             \
			);                                                           \
			fprintf(                                                     \
			    stdout, BLU "└[msg]: " NOCOL msg NOCOL "\n\n",           \
			    ##__VA_ARGS__                                            \
			);                                                           \
		}                                                                  \
	} while (0)

#else
#define CCOLL_LOG(msg, ...)                                            \
	do {                                                                     \
	} while (0)

#define CCOLL_WARN(msg, ...)                                           \
	do {                                                                     \
	} while (0)
#define CCOLL_ERROR(msg, ...)                                          \
	do {                                                                     \
	} while (0)

#define CCOLL_ASSERT(CONDITION, msg, ...)                              \
	do {                                                                     \
	} while (0)

#endif
