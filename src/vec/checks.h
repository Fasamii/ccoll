#if CCOLL_VEC_ARG_CHECK
#define CCOLL_VEC_INTEGRITY_CHECK(ptr)                                     \
	if (!(ptr)) {                                                            \
		CCOLL_VEC_ERROR("passed MiniVec is NULl");                     \
		return CCOLL_NULL;                                                 \
	};                                                                       \
	if (!(ptr)->data) {                                                      \
		CCOLL_VEC_ERROR("data of passed MiniVec is NULL");             \
		return CCOLL_NULL_INTERNAL_DATA;                                   \
	}

#define CCOLL_VEC_ITEM_SIZE_CHECK_NULL(sizeof_item)                        \
	if (((sizeof_item) == 0)) {                                              \
		CCOLL_VEC_ERROR("element size 0 is not allowed");              \
		return CCOLL_INVALID_ARGUMENT;                                     \
	}

#define CCOLL_VEC_CHECK_OVERFLOW_ADD(a, b)                                 \
	if (SIZE_MAX - a < b) {                                                  \
		CCOLL_VEC_ERROR("overflow");                                   \
		return CCOLL_OVERFLOW;                                             \
	}
#else
#define CCOLL_VEC_INTEGRITY_CHECK(ptr)
#define CCOLL_VEC_ITEM_SIZE_CHECK_NULL(sizeof_item)
#define CCOLL_VEC_CHECK_OVERFLOW_ADD(a, b)
#endif
