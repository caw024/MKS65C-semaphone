all:  control.c setup.c
	gcc -o main.out control.c
	gcc setup.c

setup: a.out
	./a.out $(args)

run:
	./main.out
