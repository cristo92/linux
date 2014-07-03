#!/bin/bash
check_ret() {
	if [ $? -eq 0 ]
	then
		printf "Test %s: [\e[32mOK\e[0m]\n" $1
	else
		printf "Test %s: [\e[31mERROR\e[0m]\n" $1
	fi
}

#Test1
#ioctl na niepustym pliku
rm -f /root/file2
echo "dfsagfasgdsag" > /root/file2
printf "Test1: "
./test1
check_ret 1

#Test2
# open na pliku bez prawa
rm -f /root/file3
printf "Test2: "
./test2a
if [ $? -ne 0 ]
then
	printf "[\e[31mERROR\e[0m]\n"
else 
	./test2b
	check_ret 2
fi

# Test3
# ioctl bez klucza
rm -f /root/file3
printf "Test3: "
./test3
check_ret 3

# Test4
# ioctl ze zlym kluczem
rm -f /root/file3
printf "Test4: "
./test4
check_ret 4

# Test5
# ioctl z dobrym kluczem na szyfrowanym pliku
rm -f /root/file3
printf "Test5: "
./test2a
check_ret 5a
./test5
check_ret 5b

# Test6
# Test na read pliku 16B (jedna strona)
rm -f ~/file4
./test6a
check_ret 6a
sync
./test6b
check_ret 6b
sync
./test6c
check_ret 6c
sync
./test6d
check_ret 6d

# Test7
# Test na read pliku 16kB (wiele stron)
rm -f ~/file5
./test7a
check_ret 7a
sync
./test7b
check_ret 7b

# Test 8
# Test wielowatkowego pisania i czytania
echo 3 | tee /proc/sys/vm/drop_caches
./test6b & ./test6c & ./test6b & ./test6c & ./test6d & \
./test6b & ./test6c & ./test6b & ./test6c & ./test6d & \
./test6b & ./test6c & ./test6b & ./test6c & ./test6d
check_ret 8


