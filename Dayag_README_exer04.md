# Laboratory Exercise 04 Part 01

Van Paul Angelo C. Dayag | 2020-10106

CMSC 180 T6L

# Installation

Programming language: **C**  

Dependencies: **gcc**

Optional Dependencies: **make**

# Tested Environments
 - gcc 12.2.1 on Arch Linux
 - gcc 11.3.0 on Ubuntu 22.04.1

# Compile & Run

### Exercise 1: Single thread implementation:

```
make exer01
```
or
```
gcc interpolation.c -o interpolation
./interpolation n
```

### Exercise 2: Multithreaded implementation:

```
make exer02
```
or
```
gcc -Iheaders/auto interpolation-multithread.c -pthread -o interpolation-multithread
./interpolation-multithread n t
```

### Exercise 3: Multithreaded implementation with Core affinity:

```
make exer03
```
or
```
gcc -Iheaders/manual interpolation-multithread.c -pthread -o interpolation-multithread
./interpolation-multithread n t
```

### Exercise 3: Distributing Parts of a Matrix over Sockets:

```
make exer04
```

```
./master n config_filename
```

```
./slave port
```