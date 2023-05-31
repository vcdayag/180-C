default: run-single

compile-multi-opt:
	gcc -O2 interpolation-multithread.c -pthread -o IM-OPTIMIZED

compile-single:
	gcc interpolation.c -o interpolation

compile-auto:
	gcc -Iheaders/auto interpolation-multithread.c -pthread -o interpolation-multithread

compile-manual:
	gcc -Iheaders/manual interpolation-multithread.c -pthread -o interpolation-multithread

compile-exer01: compile-single

compile-exer02: compile-auto

compile-exer03: compile-manual

exer01: compile-exer01
	./interpolation 16000 && \
	rm ./interpolation

exer02: compile-exer02
	./interpolation-multithread 16000 8 && \
	rm ./interpolation-multithread

exer03: compile-exer03
	./interpolation-multithread 16000 8 && \
	rm ./interpolation-multithread


compile-master:
	gcc master.c -o master

compile-slave:
	gcc slave.c -o slave

compile-exer04:
	gcc -c master.c
	gcc -c slave.c
	gcc -c interpolation.c
	gcc -c distributed.c
	gcc -o master master.o interpolation.o distributed.o
	gcc -o slave slave.o interpolation.o distributed.o
	rm *.o

run-master: compile-exer04
	./master 10 ./config_2

run-slave: compile-exer04
	./slave 5050

distrib:
	gcc distributed.c -o distributed
	./distributed