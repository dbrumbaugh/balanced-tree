bst-test: bst-test.c bst.o
	gcc bst-test.c bst.o -o bst-test

bst.o: bst.c bst.h
	gcc -c bst.c -o bst.o

clean:
	rm bst.o bst-test
