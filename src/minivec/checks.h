#if CCOLL_MINIVEC_ARG_CHECK

#define CCOLL_MINIVEC_CHECK_MINIVEC_INTEGRITY(ptr)                             \
	if (!(ptr)) {                                                            \
		CCOLL_MINIVEC_ERROR("passed MiniVec is NULl");                     \
		return CCOLL_NULL;                                                 \
	};                                                                       \
	if (!(ptr)->data) {                                                      \
		CCOLL_MINIVEC_ERROR("data of passed MiniVec is NULL");             \
		return CCOLL_NULL_INTERNAL_DATA;                                   \
	}

#define CCOLL_MINIVEC_CHECK_CAPACITY(capacity, item_size)                      \
	if (capacity == 0) {                                                     \
		CCOLL_MINIVEC_WARN("cannot request capacity of size 0");           \
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;                          \
	}                                                                        \
	if (MiniVec_mul_will_overflow(capacity, item_size)) {                    \
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");              \
		return CCOLL_OVERFLOW;                                             \
	}

#define CCOLL_MINIVEC_CHECK_ITEM_SIZE(item_size)                          \
	if ((item_size) == 0) {                                                  \
		CCOLL_MINIVEC_ERROR("passed item_size 0");                         \
	}

#define CCOLL_MINIVEC_CHECK_ALIGNMENT(alignment)                               \
	if (alignment < CCOLL_MINIVEC_MIN_ALIGNMENT) {                           \
		CCOLL_MINIVEC_ERROR(                                               \
		    "passed aalignment is smaller than "                           \
		    "CCOLL_MINIVEC_MIN_ALIGNMENT"                                  \
		);                                                                 \
		return CCOLL_INVALID_ARGUMENT; /* TODO: make custom err */         \
	}                                                                        \
	if ((alignment & (alignment - 1)) != 0) {                                \
		CCOLL_MINIVEC_ERROR(                                               \
		    "passed alignment isn't pover of two (%zu)", capacity          \
		);                                                                 \
		return CCOLL_INVALID_ARGUMENT; /*TODO: make custom err */          \
	}

#define CCOLL_MINIVEC_CHECK_OVERFLOW_ADD(a, b)                                 \
	if (SIZE_MAX - a < b) {                                                  \
		CCOLL_MINIVEC_ERROR("overflow");                                   \
		return CCOLL_OVERFLOW;                                             \
	}

#else

#define CCOLL_MINIVEC_CHECK_MINIVEC_INTEGRITY(ptr)
#define CCOLL_MINIVEC_CHECK_CAPACITY(capacity, item_size, size)
#define CCOLL_MINIVEC_CHECK_ALIGNMENT(alignment)

#endif
