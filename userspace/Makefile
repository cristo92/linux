addkey:
	gcc -o a.out /hshare/testaddkey.c

bio: 
	gcc -o bio_submit_test1 bio_submit_test1.c
	gcc -o bio_submit_test2 bio_submit_test2.c

tests: tests/test1.o

tests/test1.o: tests/test1.c
	gcc -o tests/test1.o tests/test1.c

