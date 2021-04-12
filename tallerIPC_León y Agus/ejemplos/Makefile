.PHONY: clean all

BIN = \
	pipe                     \
	dup2				 \

SRC = $(BIN:=.c)

.c:
	$(CC) -D_POSIX_C_SOURCE=200809L $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

all: $(BIN)

clean:
	rm -f $(BIN)
