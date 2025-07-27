SHELL := /bin/bash
CC = gcc
FLAG =-Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -g
FILE = TESTS/test_1_cat.txt
SRCS = s21_cat.c s21_cat_functions.c
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all
TARGET = s21_cat
ERROR = ERROR

all: s21_cat tests

s21_cat: $(SRCS)
	$(CC) $(FLAG) $(SRCS) -o $(TARGET)

tests:
	-diff <(./$(TARGET) $(FILE)) <(cat $(FILE))
	-diff <(./$(TARGET) -e $(FILE)) <(cat -e $(FILE))
	-diff <(./$(TARGET) -n $(FILE)) <(cat -n $(FILE))
	-diff <(./$(TARGET) -t $(FILE)) <(cat -t $(FILE))
	-diff <(./$(TARGET) -s $(FILE)) <(cat -s $(FILE))
	-diff <(./$(TARGET) --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./$(TARGET) --number $(FILE)) <(cat -n $(FILE))
	-diff <(./$(TARGET) -b $(FILE)) <(cat -b $(FILE))
	-diff <(./$(TARGET) --number-nonblank $(FILE)) <(cat -b $(FILE))
	-diff <(./$(TARGET) -etsnb $(FILE)) <(cat -etsnb $(FILE))

	echo "Testing for memory leaks"
	$(CHECK) ./$(TARGET) $(FILE)

clean:
	rm -f $(TARGET)