
default: prog

get-deps-apt:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

get-deps-brew:
	# Assuming Mac OS
	brew install check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall -c spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

main.o: main.c
	gcc -Wall -c main.c

test: dictionary.o spell.o test_main.o
	gcc -Wall -g -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

test-valgrind: dictionary.o spell.o test_main.o
	gcc -Wall -g -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	valgrind --leak-check=full ./test_main

prog: cleanall dictionary.o spell.o main.o
	gcc -Wall -g -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o

cleanall:clean
	rm spell_check