all: tests

OBJ = test1 test2
tests: $(OBJ)

%: %.c
	gcc -o $@ $<

clean: 
	rm -f $(OBJ)
