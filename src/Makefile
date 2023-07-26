.PHONY: all clean main tests hash_table.a self_balancing_binary_search_tree.a clang cppcheck leaks valgrind gcov_report
PROJECT_NAME = transactions

CC = g++ -std=c++17
WARNINGS_FLAGS = -Wall -Werror -Wextra
CPPCHECK_FLAGS = --enable=all --language=c++ --std=c++17 --suppress=missingInclude
TEST_LIB = -lgtest

SOURCES_C := *.cc model/*/*.cc interface/*.cc
SOURCES_H := *.h model/*/*.h interface/*.h

OS = $(shell uname)
ifeq ($(OS),Darwin)
	GCOVFLAGS = --coverage
else
	GCOVFLAGS = -fprofile-arcs -ftest-coverage
endif

all: clean main

clean:
	rm -f *.o *.gcno *.gcda *.info *.out */.clang-format .clang-format *.a model/*.gch
	rm -rf ./report/ *.dSYM
	@clear

main: clean
	$(CC) model/hashTable/*.cc model/tree/self_balancing_binary_search_tree.cc interface/*.cc main.cc -o main.out
	./main.out

tests: clean
	$(CC) model/hashTable/*.cc model/tree/self_balancing_binary_search_tree.cc test.cc $(TEST_LIB) -o test.out
	./test.out

hash_table.o: model/hashTable/*.cc
	$(CC) -c $^

hash_table.a: hash_table.o
	ar rcs hash_table.a hashTable.o
	ranlib hash_table.a

self_balancing_binary_search_tree.o: 
	$(CC) -c model/tree/self_balancing_binary_search_tree.cc -o self_balancing_binary_search_tree.o 

self_balancing_binary_search_tree.a: self_balancing_binary_search_tree.o
	ar rcs self_balancing_binary_search_tree.a self_balancing_binary_search_tree.o
	ranlib self_balancing_binary_search_tree.a

clang:
	@cp ../materials/linters/.clang-format ./
	@clang-format -i */*.cc
	@clang-format -i */*.h
	clang-format -n */*.cc
	clang-format -n */*.h
	@rm -f .clang-format

cppcheck:
	@cppcheck $(CPPCHECK_FLAGS) $(SOURCES_C) $(SOURCES_H) test.cc main.cc

leaks: clean
	$(CC) -g model/hashTable/*.cc model/tree/self_balancing_binary_search_tree.cc test.cc $(TEST_LIB) -o test.out
	CK_FORK=no leaks --atExit -- ./test.out

valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./test.out

gcov_report: clean
	$(CC) $(GCOVFLAGS) model/hashTable/*.cc model/tree/self_balancing_binary_search_tree.cc test.cc $(TEST_LIB) -o gcov_rep.out
	./gcov_rep.out
	lcov -t "gcov_rep" --no-external --output-file=coverag.info --capture --directory .
	genhtml -o ./report coverag.info
ifeq ($(OS),Darwin)
	open ./report/index.html
endif
