SRC_DIR=src
OUT_DIR=bin
OBJ_DIR=$(OUT_DIR)/obj

OUTNAME=ctp

CC=gcc
CC_FLAGS=-Wall -Wextra -c
LC_FLAGS=-I$(SRC_DIR) -Wall -Wextra 

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

.PHONY: all $(OUTNAME) clean memtest test

all: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LC_FLAGS) -o $(OUT_DIR)/$(OUTNAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean: 
	rm -rf $(OUT_DIR)/$(OUTNAME)
	rm -rf $(wildcard $(OBJ_DIR)/*.o)

test:
	python3 test/ctp_test.py

memtest:
	valgrind -q --leak-check=full --show-reachable=yes --error-exitcode=1 $(OUT_DIR)/$(OUTNAME) ./test/test1 -q
	! valgrind -q --leak-check=full --show-reachable=yes $(OUT_DIR)/$(OUTNAME) ajndhsy20fwnoc02nsuya0d3h -q
