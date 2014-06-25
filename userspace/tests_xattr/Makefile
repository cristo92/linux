all: tests

OBJ = test1 test2a test2b test3 test4 test5 test6a test6b
tests: $(OBJ)

%: %.c
	gcc -o $@ $<

clean: 
	rm -f $(OBJ)
