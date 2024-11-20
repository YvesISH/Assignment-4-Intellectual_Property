CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lssl -lcrypto

all: blockchain

blockchain: blockchain.c
	$(CC) $(CFLAGS) blockchain.c -o blockchain $(LDFLAGS)

clean:
	rm -f blockchain
