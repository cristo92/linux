all: tests

OBJ = test1 test2a test2b test3 test4 test5
tests: $(OBJ)

%: %.c
	gcc -o $@ $<

clean: 
	rm -f $(OBJ)
