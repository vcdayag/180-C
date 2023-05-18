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


compile-server:
	gcc server.c -o server

compile-client:
	gcc client.c -o client

compile-exer04:
	gcc -c server.c
	gcc -c interpolation.c
	gcc -o server server.o interpolation.o
	rm *.o