SRC_FILES = src/readfile.c \
			src/occurrences.c \
			src/huffman_tree.c \
			src/binary_codes.c \
			src/write_compress.c \
			src/bit_buffer.c
TESTS_FILES = 	

SRC_OBJS = $(SRC_FILES:.c=.o)
TESTS_OBJS = $(TESTS_FILES:.c=.o)

BIN_NAME = bin/huffman
DBG_BIN_NAME = bin/huffman_debug
TESTS_NAME = bin/huffman_tests

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
				gcc -o $(BIN_NAME) $(SRC_OBJS) src/main.o $(GCC_FLAGS)

$(DBG_BIN_NAME):	$(SRC_OBJS) src/main.o
					gcc -o $(DBG_BIN_NAME) $(SRC_OBJS) src/main.o $(GCC_FLAGS) $(DBG_FLAGS)

unit_tests: clean $(BIN_NAME) $(TESTS_OBJS)
		gcc -o $(TESTS_NAME) $(TESTS_OBJS) $(GCC_FLAGS) $(TESTS_FLAGS)

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
		rm -f $(BIN_NAME)
		rm -f $(TESTS_NAME)
