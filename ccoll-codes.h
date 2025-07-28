// TODO: after making other ccoll's make sure that errors are consistent, and
// overall redo error codes and names

// You need to make error codes for:
// - Success -> When everything was ok
// - Out of memory -> on malloc or realloc fail's
// - Destroyed -> If foo is successful but data structure was destroyed (freed)
// - CCOLL Overflow -> self explanatory
// +> callback behaviours
//	- Success but omitted one element
//	- Canceled -> Canceled by on_remove fn return code
// +> For invalid input data:
//	 - ccoll is NULL -> when reference to data structure is NULL
//	 - not enough memory requested
//	 - data structure is empty

#ifndef CCOLL_ERROR_CODES_H
#define CCOLL_ERROR_CODES_H

#define CCOLL_SUCCESS 0
#define CCOLL_CANCELED 10
#define CCOLL_ERROR -1
#define CCOLL_DESTROYED 1

#define CCOLL_EMPTY -20
#define CCOLL_OVERFLOW -30

#define CCOLL_OUT_OF_MEMORY -40
#define CCOLL_NO_REMOVED_MEMORY 40
#define CCOLL_ELEMENT_SIZE_MISMATCH -45

#define CCOLL_INVALID_ARGUMENT -50
#define CCOLL_NOT_ENOUGH_MEMORY_REQUESTED -50
#define CCOLL_NULL -52
#define CCOLL_NULL_INTERNAL_DATA -53
#define CCOLL_NULL_DATA -54
#define CCOLL_NULL_FN -54
#define CCOLL_INVALID_ELEMENT -55
#define CCOLL_INVALID_RANGE -56

#endif

#ifndef CCOLL_OPERATION_CODES_H
#define CCOLL_OPERATION_CODES_H

typedef enum CCOLL_OPERATION {
	CCOLL_OPERATION_CREATE,		   // creating new element
	CCOLL_OPERATION_REPLACE,	   // replacing element
	CCOLL_OPERATION_REPLACE_FORCED, // replacing (unstoppable)
	CCOLL_OPERATION_REMOVE,		   // removing element
	CCOLL_OPERATION_REMOVE_FORCED,   //  removing (unstoppable)
} CCOLL_OPERATION;

#endif

#ifndef CCOLL_CALLBACK_CODES_H
#define CCOLL_CALLBACK_CODES_H

typedef enum CCOLL_CALLBACK {
	CCOLL_CALLBACK_NOTHING,
	CCOLL_CALLBACK_CANCEL,
	CCOLL_CALLBACK_DESTROY_VEC,
	CCOLL_CALLBACK_REMOVE,
	CCOLL_CALLBACK_SKIP_NEXT,
} CCOLL_CALLBACK;

#endif
