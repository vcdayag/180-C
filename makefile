default: run-single

compile-multi:
	gcc interpolation.c -pthread -o interpolation

compile-single:
	gcc interpolation.c -o interpolation

run-single: compile-single
	./interpolation 10