all: head

head: main.o mpp2file.o
	gcc -Wall -Wextra -std=gnu99 -pedantic -Werror main.o mpp2file.o -o head

main.o: main.c
	gcc -c main.c

mpp2file.o: mpp2file.c
	gcc -c mpp2file.c

clean:
	rm -rf *.o head
