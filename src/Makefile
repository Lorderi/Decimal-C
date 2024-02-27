CC = gcc
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CHECK_DECIMAL = $(APP_SOURCES) -lcheck
LEAK_CMD = CK_FORK=no leaks -atExit -- ./$(TEST_PATH)
endif
ifeq ($(UNAME), Linux)
CHECK_DECIMAL = $(APP_SOURCES) -lcheck -lm -lpthread -lrt -lsubunit -D_GNU_SOURCE
LEAK_CMD = CK_FORK=no valgrind --leak-check=full ./$(TEST_PATH)
endif
CFLAGS = -std=c11 -Wall -Wextra -Werror -I $(SRC_DIR)
GCOV_FLAGS = --coverage
CLANG-FORMAT-PATH = ../materials/linters/.clang-format
TEST_PATH = test
APP_LIB_PATH = decimal.a
SRC_EXT = c
SRC_DIR = .
OBJ_DIR = .
GCOV_DIR = .
TEST_FILE = main_test.c

APP_SOURCES = $(shell find $(SRC_DIR) -name '*.$(SRC_EXT)' ! -name '*test.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.o)

.PHONY: all
all: clean $(APP_LIB_PATH)

$(APP_LIB_PATH): $(APP_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TEST_PATH): clean $(APP_SOURCES) $(TEST_FILE)
	$(CC) $(CFLAGS) $(TEST_FILE) $(CHECK_DECIMAL) -o $@ && ./test

.PHONY: format
format:
	ls $(SRC_DIR)/*.c | xargs clang-format -i -style=file:$(CLANG-FORMAT-PATH);
	ls $(SRC_DIR)/*.h | xargs clang-format -i -style=file:$(CLANG-FORMAT-PATH);

.PHONY: test_format
test_format:
	ls $(SRC_DIR)/*.c | xargs clang-format -n -style=file:$(CLANG-FORMAT-PATH);
	ls $(SRC_DIR)/*.h | xargs clang-format -n -style=file:$(CLANG-FORMAT-PATH);

.PHONY: leaks
leaks:  #  $(TEST_PATH)
	$(LEAK_CMD)

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

.PHONY: gcov_report
gcov_report: clean gcov_build
	./test; lcov -t "$(APP_PATH)" -o decimal.info -c -d .
	genhtml -o report decimal.info

.PHONY: gcov_build
gcov_build: $(APP_SOURCES) $(TEST_FILE)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_FILE) $(CHECK_DECIMAL) -o $(TEST_PATH)

.PHONY: clean
clean:
	$(RM) $(APP_PATH) decimal.info $(APP_LIB_PATH) test
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(SRC_DIR) -name '*.gcov' -exec $(RM) '{}' \;
	find $(SRC_DIR) -name '*.gcda' -exec $(RM) '{}' \;
	find $(SRC_DIR) -name '*.gcno' -exec $(RM) '{}' \;
	rm -rf report;
