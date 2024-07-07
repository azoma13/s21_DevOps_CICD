all: s21_grep
TARGET1 = s21_grep

CC = gcc -Wall -Wextra -Werror -g 

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET1) : s21_grep.o
	$(CC) s21_grep.o -o $(TARGET1)

%.o : %.c
	$(CC) -c $< -o $@

test: test_grep.sh
	sh test_grep.sh

clean:
	rm $(TARGET1) *.o

cpp:
	cp ../../materials/linters/.clang-format .
	clang-format -i *.c *.sh
	cppcheck --enable=all --suppress=missingIncludeSystem *.c