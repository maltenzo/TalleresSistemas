.PHONY: clean all

BIN = \
  pipe2                    \
  tcp-client               \
  server-blocking          \
  server-fcntl             \
  server-pselect

SRC = $(BIN:=.c)

.c:
	$(CC) -D_POSIX_C_SOURCE=200809L $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

all: $(BIN)

clean:
	rm -f $(BIN)
