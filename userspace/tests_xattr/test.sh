#!/bin/bash
check_ret() {
	if [ $? -eq 0 ]
	then
		printf "[\e[32mOK\e[0m]\n"
	else
		printf "[\e[31mERROR\e[0m]\n"
	fi
}

#Test1
#ioctl na niepustym pliku
rm -f /root/file2
echo "dfsagfasgdsag" > /root/file2
printf "Test1: "
./test1
check_ret

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
	check_ret
fi

# Test3
# ioctl bez klucza
rm -f /root/file3
printf "Test3: "
./test3
check_ret

# Test4
# ioctl ze zlym kluczem
rm -f /root/file3
printf "Test4: "
./test4
check_ret

# Test5
# ioctl z dobrym kluczem na szyfrowanym pliku
rm -f /root/file3
printf "Test5: "
./test2a
check_ret
./test5
check_ret

# Test6
# Test na read pliku 16B (jedna strona)
./test6b
check_ret

# Test7
# Test na read pliku 16kB (wiele stron)
./test7b
check_ret
