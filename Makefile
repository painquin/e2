CC=gcc
CFLAGS=-fPIC -g -Werror
LFLAGS=-llua

OFILES=main.o util.o


e2server: $(OFILES)
	$(CC) $(LFLAGS) $^ -o $@

run: e2server
	./e2server
