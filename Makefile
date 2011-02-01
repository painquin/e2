CC=gcc
CFLAGS=-fPIC -g -Werror
LFLAGS=-llua -luuid -lpthread

OFILES=main.o util.o net.o


e2server: $(OFILES)
	$(CC) $(LFLAGS) $^ -o $@

run: e2server
	./e2server
	
clean:
	rm -rf e2server *.o
