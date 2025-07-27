SHELL := /bin/bash
CC = gcc
FLAG =-Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -g
FILE = TESTS/test_1.txt
SRCS = s21_grep.c s21_grep_func.c
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all
TARGET = s21_grep
ERROR = ERROR

all: s21_grep tests

s21_grep: $(SRCS)
	$(CC) $(FLAG) $(SRCS) -o $(TARGET)

tests:
	-diff <(./$(TARGET) ap $(FILE)) <(grep ap $(FILE))
	-diff <(./$(TARGET) -e ap $(FILE)) <(grep -e ap $(FILE))
	-diff <(./$(TARGET) -i ap $(FILE)) <(grep -i ap $(FILE))
	-diff <(./$(TARGET) -v ap $(FILE)) <(grep -v ap $(FILE))
	-diff <(./$(TARGET) -c ap $(FILE)) <(grep -c ap $(FILE))
	-diff <(./$(TARGET) -l ap $(FILE)) <(grep -l ap $(FILE))
	-diff <(./$(TARGET) -n ap $(FILE)) <(grep -n ap $(FILE))

	echo "Testing for memory leaks"
	$(CHECK) ./$(TARGET) -e ap $(FILE)

clean:
	rm -f $(TARGET)
