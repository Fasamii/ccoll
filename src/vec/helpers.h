#define Vec_count_to_bytes(vec, idxs) ((vec)->item_size * (idxs))
#define Vec_bytes_to_count(vec, bytes) ((bytes) / (vec)->item_size)

#define Vec_mul_will_overflow(a, b) ((a) != 0 && (b) > SIZE_MAX / (a))

#define Vec_round_up_bytes_to_alignment(size, alignment)                   \
	(((size) + (alignment) - 1) / (alignment) * (alignment))

#define Vec_pad_bytes_to_alignment(vec, capacity, alignment)                            \
	Vec_round_up_bytes_to_alignment(                                     \
	    Vec_count_to_bytes(vec, capacity), alignment                     \
	)
