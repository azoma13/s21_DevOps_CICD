all: s21_cat
TARGET1 = s21_cat

CC = gcc -Wall -Wextra -Werror 

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET1) : s21_cat.o
	$(CC) s21_cat.o -o $(TARGET1)

%.o : %.c
	$(CC) -c $< -o $@

test: test_cat.sh
	sh test_cat.sh

clean:
	rm $(TARGET1) *.o

cpp:
	cp ../../materials/linters/.clang-format .
	clang-format -i *.c *.sh
	cppcheck --enable=all --suppress=missingIncludeSystem *.c