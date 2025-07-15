CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11
SOURCE=$(wildcard s21_*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCE))
COVERAGE=-fprofile-arcs -ftest-coverage

SYSTEM := $(shell uname -s)
ifeq ($(SYSTEM),Linux)
    LFLAGS = -lcheck -lsubunit -lm -lrt -lpthread -L. -l:s21_decimal.a
endif
ifeq ($(SYSTEM),Darwin)
	LFLAGS = -lcheck -lm -lpthread s21_decimal.a
endif

all: s21_decimal.a

gcov_report: clean
	$(CC) $(FLAGS) $(COVERAGE) -c $(SOURCE)
	ar -rcs s21_decimal.a $(OBJECTS)
	$(CC) $(FLAGS) $(COVERAGE) -c test.c -o test.o
	$(CC) $(FLAGS) $(COVERAGE) test.o -o test $(LFLAGS)
	chmod +x test
	./test
	mkdir gcov_report
	gcov test.c $(SOURCE)
	lcov -c -d ./ --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./gcov_report/

open:
	open gcov_report/index.html

test: clean test.o s21_decimal.a
	$(CC) $(FLAGS) test.o -o test $(LFLAGS)
	chmod +x test
	./test

valgrind: clean test
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --log-file=valgrind.txt ./test
	grep "errors from" valgrind.txt

test.o: test.c
	$(CC) $(FLAGS) -c test.c -o test.o

s21_decimal.a: $(OBJECTS)
	ar -rcs s21_decimal.a $(OBJECTS)

$(OBJECTS): $(SOURCE)
	$(CC) $(FLAGS) -c $(SOURCE)

clang-check:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm .clang-format

clean:
	rm -rf s21_decimal.a test *.o *.gcda *.gcno *.gcov *.gch ./gcov_report *.html *.css *.info valgrind.txt
