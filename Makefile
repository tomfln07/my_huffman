SRC_FILES = src/readfile.c \
			src/occurrences.c \
			src/huffman_tree.c \
			src/binary_codes.c \
			src/write_compress.c \
			src/bit_buffer.c \
			src/flags.c \
			src/compress.c \
			src/uncompress.c \
			src/write_uncompress.c
TESTS_FILES = 	

BIN_PATH = bin/

SRC_OBJS = $(SRC_FILES:.c=.o)
TESTS_OBJS = $(TESTS_FILES:.c=.o)

BIN_NAME = huffman
DBG_BIN_NAME = huffman_debug
TESTS_NAME = huffman_tests

GCC_FLAGS = -Iinclude -Wall -Wextra
TESTS_FLAGS = --coverage -lcriterion
DBG_FLAGS = -g -pg -fsanitize=address

all: $(BIN_NAME)

re: fclean $(BIN_NAME)

%.o: %.c
		gcc $(GCC_FLAGS) -c $< -o $@ -g -pg

$(TESTS_OBJS): %.o: %.c
		gcc $(GCC_FLAGS) -c $< -o $@

$(BIN_NAME):	$(SRC_OBJS) src/main.o
				gcc -o $(BIN_PATH)$(BIN_NAME) $(SRC_OBJS) src/main.o $(GCC_FLAGS)

$(DBG_BIN_NAME):	$(SRC_OBJS) src/main.o
					gcc -o $(BIN_PATH)$(DBG_BIN_NAME) $(SRC_OBJS) src/main.o $(GCC_FLAGS) $(DBG_FLAGS)

unit_tests: clean $(BIN_NAME) $(TESTS_OBJS)
		gcc -o $(BIN_PATH)$(TESTS_NAME) $(TESTS_OBJS) $(GCC_FLAGS) $(TESTS_FLAGS)

tests_run: unit_tests
			./unit_tests

clean:
		rm -f */**/*.o
		rm -f */*.o
		rm -f *~
		rm -f */**/*.gcno
		rm -f */*.gcno
		rm -f */**/*.gcda
		rm -f */*.gcda

fclean: clean
		rm -f $(BIN_PATH)$(BIN_NAME)
		rm -f $(BIN_PATH)$(DBG_BIN_NAME)
		rm -f $(BIN_PATH)$(TESTS_NAME)
