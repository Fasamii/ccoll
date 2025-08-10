#include "../../ccoll-codes.h"
#include "../../include/vec.h"
#include "../util.h"

const char *operation_names[] = {
    "create", "repleace", "repleace_forced", "remove", "remove_forced",
};

static bool cond = false;

int CALLBACK_remove_reange(
    void *data, size_t idx, size_t element_size, CCOLL_OPERATION op
) {
	log("\t[%ld]  get > %f at >>=>>%p <-------<operation>--------< %s", idx,
	    *(double *)data, data, operation_names[op]);
	if (cond) {
		if (idx == 0 || idx == 3 || idx == 5 || idx == 7) {
			log("\t\t\t\tcac on [%ld]", idx);
			return CCOLL_CALLBACK_CANCEL;
		}
	}
	return CCOLL_CALLBACK_NOTHING;
}

int TEST_remove_range() {
	Vec *vec = Vec_init_with(sizeof(double), 12);
	Vec_set_on_change_callback(vec, CALLBACK_remove_reange);
	double data = 12.12312312;
	double datb = 13.54638432;

	log("\t Before FILL");
	Vec_fill(vec, &data);
	log("\t AFTER FILL");

	log("\t Before set_range");
	Vec_set_range(vec, &datb, 0, 1);
	Vec_set_range(vec, &datb, 3, 1);
	Vec_set_range(vec, &datb, 6, 1);
	log("\t After set_range");

	for (size_t i = 0; i < vec->size; i++) {
		log("Vec::[%ld]::(%f)", i,
		    *(double *)Vec_get_unchecked_ptr(vec, i));
	}

	cond = true;

	log("\t Before rm_reange");
	Vec_remove_range(vec, 1, 4);
	log("\t After rm_reange");

	for (size_t i = 0; i < vec->size; i++) {
		log("Vec::[%ld]::(%f)", i,
		    *(double *)Vec_get_unchecked_ptr(vec, i));
	}

	Vec_free(vec);
	printf("No Segfault yay\n");
	return 0;
}

int CALLBACK_remove_reange_char(
    void *data, size_t idx, size_t element_size, CCOLL_OPERATION op
) {
	log("\t[%ld]  get > %c at >>=>>%p <-------<operation>--------< %s", idx,
	    *(char *)data, data, operation_names[op]);
	if (cond) {
		if (idx == 0 || idx == 3 || idx == 5 || idx == 7) {
			log("\t\t\t\tcac on [%ld]", idx);
			return CCOLL_CALLBACK_CANCEL;
		}
	}
	return CCOLL_CALLBACK_NOTHING;
}

int TEST_remove_range_char() {
	log("sc start\n");
	Vec *vec  = Vec_init_with(sizeof(char), 10);
	char *str = "0123456789";
	// generate_random_string(str, 12);
	Vec_set_range(vec, str, 0, 10);
	for (size_t i = 0; i < vec->size; i++) {
		log("Vec::[%ld]::(%c)", i, *(char *)Vec_get_unchecked_ptr(vec, i));
	}

	cond = true;
	Vec_set_on_change_callback(vec, CALLBACK_remove_reange_char);

	Vec_remove_range(vec, 1, 2);
	Vec_remove_range(vec, 3, 5);
	for (size_t i = 0; i < vec->size; i++) {
		log("Vec::[%ld]::(%c)", i, *(char *)Vec_get_unchecked_ptr(vec, i));
	}

	Vec_free(vec);
	return 0;
}
