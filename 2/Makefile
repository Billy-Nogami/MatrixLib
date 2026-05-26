CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = -lm

SRC = \
	src/list_memory.c \
	src/list_access.c \
	src/list_basic.c \
	src/list_advanced.c

TEST_SRC = tests/test_list.c

all: test_list

test_list: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $@ $(LDFLAGS)

test: test_list
	./test_list

clean:
	rm -f test_list

.PHONY: all test clean
