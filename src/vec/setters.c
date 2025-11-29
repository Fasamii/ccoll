#include "../../include/vec.h"
#include "./checks.h"
#include "../../util/debug.h"

#include <stdlib.h>
#include <sys/types.h>

int Vec_set(Vec *vec, const size_t idx, const void *data) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);

	if (vec->size >= vec->capacity) {
			
	}
	if (idx == vec->size) {

	}

	return -1;
}
