CFLAGS= -Wall -g -coverage

test:test.o rbtree.o
	gcc -o $@ $^ -lgcov

clean:
	rm -f *.o *.gc*

c:clean

