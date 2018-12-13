all:  control.c setup.c
	gcc -o main.out control.c
	gcc setup.c

setup -c: setup.c
	./a.out -c

setup -r: setup.c
	./a.out -r

setup -v: setup.c
	./a.out -v

run:
	./main.out
