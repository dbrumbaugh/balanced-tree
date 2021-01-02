tests: avl-test.c avl.o bst-test.c bst.o tracker.o bst-util.o
	gcc avl-test.c avl.o bst.o tracker.o bst-util.o -o avl-test -ggdb
	gcc bst-test.c bst.o tracker.o bst-util.o -o bst-test -ggdb -O0

bst-util.o: bst-util.c
	gcc -c bst-util.c -o bst-util.o -ggdb -O0

avl.o: avl.c
	gcc -c avl.c -o avl.o -ggdb

bst.o: bst.c
	gcc -c bst.c -o bst.o -ggdb -O0

tracker.o: tracker.c
	gcc -c tracker.c -o tracker.o -ggdb -O0

clean:
	rm bst-test avl-test *.o
