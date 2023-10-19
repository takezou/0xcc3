CFLAGS=-std=c11 -g -static

0xcc3: 0xcc3.c

test: 0xcc3
	./test.sh

clean:
	rm -f 9cc *.0 *~ tmp*

.PHONY: test clean
