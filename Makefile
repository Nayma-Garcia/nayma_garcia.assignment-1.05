test: assignment1.05.c 
	gcc -Wall -Werror assignment1.05.c -o test -lm -lncurses

clean:
	rm -f test