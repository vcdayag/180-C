default: run-single

compile-multi:
	gcc interpolation-multithread.c -pthread -o interpolation-multithread

compile-single:
	gcc interpolation.c -o interpolation

run-single: compile-single
	./interpolation 10000

run-multi: compile-multi
	./interpolation-multithread 40000 1