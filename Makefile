all: master program1 program2 program3

master: master.c
	gcc master.c -o master -lpthread

program1: program1.c
	gcc program1.c -o program1 -lpthread

program2: program2.c
	gcc program2.c -o program2 -lpthread

program3: program3.c
	gcc program3.c -o program3 -lpthread
