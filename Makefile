CC=gcc
CFLAGS=-fPIC -g -Werror
LFLAGS=-llua -luuid -lpthread

NET_FILES=net-server.o net-packing.o net-incoming-message.o net-outgoing-message.o net-names.o


OFILES=main.o util.o $(NET_FILES)

e2server: $(OFILES)
	$(CC) $(LFLAGS) $^ -o $@

net-client-test: util.o $(NET_FILES) net-client-test.o
	$(CC) $(LFLAGS) $^ -o $@

run: e2server
	./e2server
	
clean:
	rm -rf e2server *.o
