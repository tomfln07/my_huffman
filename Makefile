SRC_FILES = src/readfile.c \
			src/binary_codes.c \
			src/bit_buffer.c \
			src/flags.c \
			src/compress.c \
			src/uncompress.c \
			src/occurrences/occurrences.c \
			src/occurrences/occurr_array_utils.c \
			src/huffman_tree/gen_tree.c \
			src/huffman_tree/tree_utils.c \
			src/write_files/write_compress.c \
			src/write_files/write_uncompress.c \
			src/file_name.c
TESTS_FILES =

BIN_PATH = bin/

SRC_OBJS = $(SRC_FILES:.c=.o)
TESTS_OBJS = $(SRC_FILES:.c=.test.o) $(TESTS_FILES:.c=.test.o)
DBG_SRC_OBJS = $(SRC_FILES:.c=.dbg.o)

BASE_NAME = my_huffman
BIN_NAME = $(BASE_NAME)
DBG_BIN_NAME = $(BASE_NAME)_debug
TESTS_NAME = $(BASE_NAME)_tests

GCC_FLAGS = -Iinclude -Wall -Wextra
TESTS_FLAGS = --coverage -lcriterion
DBG_FLAGS = -g -pg

all: $(BIN_NAME)

dbg: $(DBG_BIN_NAME)

re: fclean $(BIN_NAME)

%.o: %.c
		gcc $(GCC_FLAGS) -c $< -o $@

%.dbg.o: %.c
	gcc $(GCC_FLAGS) $(DBG_FLAGS) -c $< -o $@

%.test.o: %.c
	gcc $(GCC_FLAGS) $(TESTS_FLAGS) -c $< -o $@

$(BIN_NAME):	$(SRC_OBJS) src/main.o
				gcc -o $(BIN_PATH)$(BIN_NAME) $(SRC_OBJS) src/main.o $(GCC_FLAGS)

$(DBG_BIN_NAME):	$(DBG_SRC_OBJS) src/main.dbg.o
					gcc -o $(BIN_PATH)$(DBG_BIN_NAME) $(DBG_SRC_OBJS) src/main.dbg.o $(GCC_FLAGS) $(DBG_FLAGS)

unit_tests: clean $(TESTS_OBJS)
		gcc -o $(BIN_PATH)$(TESTS_NAME) $(TESTS_OBJS) $(GCC_FLAGS) $(TESTS_FLAGS)

tests_run: unit_tests
			./unit_tests

clean:
		find . -type f -name "*.o" -delete
		find . -type f \( -name "*.gcno" -o -name "*.gcda" \) -delete
		find . -type f -name "*~" -delete

fclean: clean
		rm -f $(BIN_PATH)$(BIN_NAME) $(BIN_PATH)$(DBG_BIN_NAME) $(BIN_PATH)$(TESTS_NAME)
