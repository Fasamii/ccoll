# Implementation of C cool collections
right now Vec (vector) is being implemented
# How to build
```bash
make
```
or if you want .a files of specific collection use
```bash
make [collection name]
```
e.g.:
```bash
make vec 
```
# How to use
1. you have to #include .h file from ccoll/include/(collection name).h in your program. Each
collection have its own .h file
2. then you have to build your project including in CC command .a file from ccoll/build/lib/ of used collections or if you
want all just include ccoll/build/lib/libccoll.a file which contains all .a files, you can build it with 
```bash
make ccoll # build's main libccoll.a file
```
or
```bash
make all # build's all .a files
```
# overall TODO's
- [ ] write implementation of: linked list, ring linked list, ring vec, queue, stack
- [ ] make API more consistent
- [ ] make get foo's have variants like get last and first
- [ ] rewrite the .h files to group similar foo's and make better explanations
- [ ] make some error logging functionality that can be disabled in build time
- [ ] fix after_rm foo functionality and make vec call it on every element removal
