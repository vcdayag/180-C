default: run-single

compile-multi-opt:
	gcc -O2 interpolation-multithread.c -pthread -o IM-OPTIMIZED

compile-single:
	gcc interpolation.c -o interpolation

compile-auto:
	gcc -Iheaders/auto interpolation-multithread.c -pthread -o interpolation-multithread

compile-manual:
	gcc -Iheaders/manual interpolation-multithread.c -pthread -o interpolation-multithread

compile-exer02: compile-auto

compile-exer03: compile-manual

single: compile-single
	./interpolation 16000 && \
	rm ./interpolation

exer02: compile-exer02
	./interpolation-multithread 16000 8 && \
	rm ./interpolation-multithread

exer03: compile-exer03
	./interpolation-multithread 16000 8 && \
	rm ./interpolation-multithread