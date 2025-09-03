CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRCS = src/tp0.c main.c
TARGET = tp0
TEST_SRC = test_tp0.c

.PHONY: all test valgrind format ci clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

test: $(TEST_SRC) src/tp0.c
	$(CC) $(CFLAGS) $(TEST_SRC) src/tp0.c -o test_tp0 && ./test_tp0

format:
	clang-format -i -style=file src/tp0.c src/tp0.h main.c test_tp0.c

ci: all test valgrind
    @echo "=== CI: build, tests & valgrind completed ==="

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) ejemplos/normal.csv

clean:
	rm -f $(TARGET) test_tp0
