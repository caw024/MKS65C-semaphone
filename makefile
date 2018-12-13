all:  control.c setup.c
	gcc -o main.out control.c
	gcc setup.c

	ifeq (setup,$(firstword $(MAKECMDGOALS)))
	  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
	  $(eval $(RUN_ARGS):;@:)
	endif


setup: setup.c
	./a.out

.PHONY: setup
	run : setup
	@echo setup $(RUN_ARGS)	

run:
	./main.out
