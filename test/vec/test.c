#include "../../ccoll_errors.h"
#include "../../colors.h"
#include "../../include/vec.h"

#include <stdio.h>

int test_init_and_free() {
	printf(BLU "INIT/FREE: Start:" NOCOL "\n");
	if(Vec_free(Vec_init(0))) printf(RED"FAIL: On free for 0"NOCOL"\n");
	printf(GRN"0B - DONE\n");
	if(Vec_free(Vec_init(4))) printf(RED"FAIL: On free for 4"NOCOL"\n");
	printf(GRN"4B - DONE\n");
	if(Vec_free(Vec_init(8))) printf(RED"FAIL: On free for 8"NOCOL"\n");
	printf(GRN"8B - DONE\n");
	if(Vec_free(Vec_init(12))) printf(RED"FAIL: On free for 12"NOCOL"\n");
	printf(GRN"12B - DONE\n");
	if(Vec_free(Vec_init(16))) printf(RED"FAIL: On free for 16"NOCOL"\n");
	printf(GRN"16B - DONE\n");
	if(Vec_free(Vec_init(32))) printf(RED"FAIL: On free for 32"NOCOL"\n");
	printf(GRN"32B - DONE\n");
	if(Vec_free(Vec_init(64))) printf(RED"FAIL: On free for 64"NOCOL"\n");
	printf(GRN"64B - DONE\n");
	if(Vec_free(Vec_init(128))) printf(RED"FAIL: On free for 128"NOCOL"\n");
	printf(GRN"12B - DONE\n");
	if(Vec_free(Vec_init(245))) printf(RED"FAIL: On free for 245"NOCOL"\n");
	printf(GRN"245B - DONE\n");
	if(Vec_free(Vec_init(512))) printf(RED"FAIL: On free for 512"NOCOL"\n");
	printf(GRN"512B - DONE\n");
	return 0;
}

int main(void) {
	test_init_and_free();
	return 0;
}
