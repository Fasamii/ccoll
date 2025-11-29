#if CCOLL_VEC_ARG_CHECK

#include "./helpers.h"

#define CCOLL_VEC_CHECK_VEC_INTEGRITY(ptr)                             \
	if (!(ptr)) {                                                            \
		CCOLL_ERROR("passed Vec is NULl");                     \
		return CCOLL_NULL;                                                 \
	};                                                                       \
	if (!(ptr)->data) {                                                      \
		CCOLL_ERROR("data of passed Vec is NULL");             \
		return CCOLL_NULL_INTERNAL_DATA;                                   \
	}

#define CCOLL_VEC_CHECK_CAPACITY(capacity, item_size)                      \
	if (capacity == 0) {                                                     \
		CCOLL_WARN("cannot request capacity of size 0");           \
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;                          \
	}                                                                        \
	if (Vec_mul_will_overflow(capacity, item_size)) {                    \
		CCOLL_ERROR("bytes multiplication overflow");              \
		return CCOLL_OVERFLOW;                                             \
	}

#define CCOLL_VEC_CHECK_ITEM_SIZE(item_size)                          \
	if ((item_size) == 0) {                                                  \
		CCOLL_ERROR("passed item_size 0");                         \
	}

#define CCOLL_VEC_CHECK_ALIGNMENT(alignment)                               \
	if (alignment < CCOLL_VEC_MIN_ALIGNMENT) {                           \
		CCOLL_ERROR(                                               \
		    "passed aalignment is smaller than "                           \
		    "CCOLL_VEC_MIN_ALIGNMENT"                                  \
		);                                                                 \
		return CCOLL_INVALID_ARGUMENT; /* TODO: make custom err */         \
	}                                                                        \
	if ((alignment & (alignment - 1)) != 0) {                                \
		CCOLL_ERROR(                                               \
		    "passed alignment isn't pover of two (%zu)", capacity          \
		);                                                                 \
		return CCOLL_INVALID_ARGUMENT; /*TODO: make custom err */          \
	}

#define CCOLL_VEC_CHECK_OVERFLOW_ADD(a, b)                                 \
	if (SIZE_MAX - a < b) {                                                  \
		CCOLL_ERROR("overflow");                                   \
		return CCOLL_OVERFLOW;                                             \
	}

#else

#define CCOLL_VEC_CHECK_VEC_INTEGRITY(ptr)
#define CCOLL_VEC_CHECK_CAPACITY(capacity, item_size, size)
#define CCOLL_VEC_CHECK_ALIGNMENT(alignment)

#endif
