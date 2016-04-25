.PHONY: test

test:
	gcc -std=c99 test.c 
clean:
	rm -f *.o test
