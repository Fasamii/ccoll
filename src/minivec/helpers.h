#define MiniVec_count_to_bytes(vec, idxs) ((vec)->item_size * (idxs))
#define MiniVec_bytes_to_count(vec, bytes) ((bytes) / (vec)->item_size)

#define MiniVec_mul_will_overflow(a, b) ((a) != 0 && (b) > SIZE_MAX / (a))

#define MiniVec_round_up_bytes_to_alignment(size, alignment)                   \
	(((size) + (alignment) - 1) / (alignment) * (alignment))

#define MiniVec_pad_bytes_to_alignment(vec, capacity, alignment)                            \
	MiniVec_round_up_bytes_to_alignment(                                     \
	    MiniVec_count_to_bytes(vec, capacity), alignment                     \
	)
