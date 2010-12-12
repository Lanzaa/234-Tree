CC=gcc
CFLAGS=-Wall -std=c99

treebuilder:
	$(CC) $(CFLAGS) -o 234Tree 234tree.c

clean:
	rm -f 234Tree
