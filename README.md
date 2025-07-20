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
you have to #include .h files from include directory of the project in C program. Each
collection have its own .h file
then you have to build your project including in CC command .a file of used collections or if you
want all just include libccoll.a file which you can build with 
```bash
make ccoll
```
command (that file includes all .a files of collections)
# overall TODO's
- [ ] make for all get variants of foo's pointer and owned version
- [ ] make API more consistent
- [ ] make get foo's variants like get last and first
- [ ] refractor the .h files to group similar foo's
