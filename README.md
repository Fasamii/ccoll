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
collection have its own .h file.
2. then you have to build your project including in CC command .a file from ccoll/build/lib/ of used collections or if you
want all just include ccoll/build/lib/libccoll.a file which you can build with 
```bash
make ccoll # build's main libccoll.a file
```
or
```bash
make all # build's all .a files
```
# overall TODO's
- [ ] make for all get variants of foo's pointer and owned version
- [ ] make API more consistent
- [ ] make get foo's variants like get last and first
- [ ] refractor the .h files to group similar foo's
- [ ] make some error logging functionality
