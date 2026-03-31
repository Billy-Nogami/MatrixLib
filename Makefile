CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = -lm

SRC = \
	src/matrix_memory.c \
	src/matrix_access.c \
	src/matrix_basic.c \
	src/matrix_advanced.c

TEST_SRC = tests/test_matrix.c

all: test_matrix

test_matrix: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $@ $(LDFLAGS)

test: test_matrix
	./test_matrix

clean:
	rm -f test_matrix

.PHONY: all test clean
