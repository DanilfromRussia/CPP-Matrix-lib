OS = $(shell uname)
GCC = gcc -lstdc++ -Wall -Werror -Wextra

ifeq ($(OS), Darwin)
	LIBFLAGS = -lm -lgtest -lstdc++
else
	LIBFLAGS=-lstdc++ `pkg-config --cflags --libs gtest`
endif

all: clean test

clean:
	rm -rf *.o *.a *.so *.cfg *.out *.dSYM test *.txt report *.info *.gcda *.gcno *.gch .clang-format

test: s21_matrix_oop.a 
	@$(GCC) s21_test.cc s21_matrix_oop.a $(LIBFLAGS)  -o test
	@./test

s21_matrix_oop.a:
	$(GCC) -c s21_matrix_oop.cc
	ar rc s21_matrix_oop.a s21_matrix_oop.o
	ranlib s21_matrix_oop.a

gcov_report: s21_matrix_oop.a
	$(GCC) --coverage s21_test.cc s21_matrix_oop.cc s21_matrix_oop.a -L. s21_matrix_oop.a  $(LIBFLAGS) -o test
	./test
	lcov -t "test" -c -d  ./ --no-external --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./report/
	open ./report/index.html

check: all
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config s21_matrix_oop.cc *.h
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n s21_matrix_oop.cc s21_test.cc s21_matrix_oop.h
	rm -rf .clang-format
ifeq ($(OS), Darwin)
	leaks --atExit -- ./test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif
