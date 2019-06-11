CC := gcc
CFLAGS := -g -Wall -Wextra

DEST := dest

.PHONY: test clean

test: $(DEST)/9cc
	./test.sh

clean:
	rm -f $(DEST)/9cc $(DEST)/*.o $(DEST)/*~ $(DEST)/tmp*

$(DEST)/9cc: src/9cc.c
	$(CC) $(CFLAGS) -o $@ $^
