default: compile

compile:
	gcc interpolation.c -pthread -o interpolation

run: compile
	./interpolation