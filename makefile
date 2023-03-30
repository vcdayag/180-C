default: run-single

compile-multi-opt:
	gcc -O2 interpolation-multithread.c -pthread -o IM-OPTIMIZED


compile-multi:
	gcc interpolation-multithread.c -pthread -o interpolation-multithread

compile-auto:
	gcc -Iheaders/auto interpolation-multithread.c -pthread -o interpolation-multithread

compile-manual:
	gcc -Iheaders/manual interpolation-multithread.c -pthread -o interpolation-multithread


compile-single:
	gcc interpolation.c -o interpolation

run-single: compile-single
	./interpolation 10000

run-multi: compile-multi
	./interpolation-multithread 20000 10

run-multi-opt: compile-multi-opt
	./IM-OPTIMIZED 20000 10