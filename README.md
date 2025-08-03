# Implementation of C cool collections
right now Vec (vector) is being implemented
# How to build
```bash
make
```
or if you want .a files of specific collection (like Vec) use
```bash
make [collection name]
```
e.g.:
```bash
make vec 
```
# How to use
1. You have to #include .h file from ccoll/include/(collection name).h in your program. Each
collection have its own .h file
2. Then you have to build your project including in CC command .a file from ccoll/build/lib/ of used collections or if you
want all just include ccoll/build/lib/libccoll.a file which contains all .a files, you can build it with 
```bash
make ccoll # build's main libccoll.a file
```
or
```bash
make all # build's all .a files
```
# overall TODO's
- [ ] consider making free foos not free and remove elements but only free you would have to remove
  range and element variant and make instead free amount variant and plain free
- [ ] change foos that are returning Vec and not Initializing it to use **new or **void (maybe
(think about it))
- [ ] write implementation of: linked list, ring linked list, ring vec, queue, stack
- [ ] make API more consistent
- [ ] make get foo's have variants like get last and first
- [ ] rewrite the .h files to group similar foo's and make better explanations
- [ ] make some error logging functionality that can be disabled in build time
- [ ] fix on_remove foo functionality and make vec call it on every element removal
- [ ] remove unnecessary checks on the top of each foo
- [ ] consider making every foo that return's pointer return result code and take ** parameter
- [ ] consider making some sort of macro for checks on the beginning of each foo
> [!INFO] memmove probably uses below optimizations by default so check if you even need to think
> about that 
- [ ] think about using 128‑bit SIMD intrinsics and AVX2 intrinsics for maximum performance
- [ ] thing about using MOVNTDQ instruction from emmintrin.h for zeroing or filling like operations
- [ ] think about using _mm_prefetch foo for large memmove operations to hint CPU cache
