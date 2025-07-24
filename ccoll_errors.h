// TODO: after making other ccoll's make sure that errors are consistent, and overall redo error codes and names

// You need to make error codes for:
// - Success -> When everything was ok
// - Out of memory -> on malloc or realloc fail's
// - Canceled -> Canceled by on_remove fn return code
// - Destroyed -> If foo is successful but data structure was destroyed (freed) 
// - CCOLL Overflow -> self explanatory
// +> For invalid input data:
//	 - ccoll is NULL -> when reference to data structure is NULL 
//	 - not enough memory requested
//	 - data structure is empty

#ifndef CCOLL_ERROR_CODES_H
#define CCOLL_ERROR_CODES_H


#define CCOLL_SUCCESS 0
#define CCOLL_ERROR -1

#define CCOLL_OUT_OF_MEMORY -2

#define CCOLL_INVALID_ARGUMENT -3
#define CCOLL_NOT_ENOUGH_MEMORY_REQUESTED -4
#define CCOLL_NULL_VEC -5
#define CCOLL_EMPTY -6

#define CCOLL_PASSED_FOO_FAIL -7
#define CCOLL_PASSED_FOO_FAIL_CONTINUED 7

#define CCOLL_OVERFLOW -8

#define CCOLL_DESTROYED 8

#endif
