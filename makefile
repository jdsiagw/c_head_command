all: head

head: main.o head.o mpp2file.o mpp2print.o
	gcc -Wall -Wextra -std=gnu99 -pedantic -Werror main.o head.o mpp2file.o mpp2print.o

main.o: main.c
	gcc -c main.c

head.o: head.c
	gcc -c head.c

mpp2file.o: mpp2file.c
	gcc -c mpp2file.c

mpp2print.o: mpp2print.c
	gcc -c mpp2print.c

clean:
	rm -rf *.o head
