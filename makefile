SRC_DIR=src
OUT_DIR=bin
OBJ_DIR=$(OUT_DIR)/obj

OUTNAME=ctp

CFLAGS=-Wall -Wextra -c
LDFLAGS=-I$(SRC_DIR) -Wall -Wextra 

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

.PHONY: all $(OUTNAME) clean memtest test

all: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUT_DIR)/$(OUTNAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	rm -rf $(OUT_DIR)/$(OUTNAME)
	rm -rf $(wildcard $(OBJ_DIR)/*.o)

test: $(OUTNAME)
	python3 test/ctp_test.py

memtest: $(OUTNAME)
	valgrind -q --leak-check=full --show-reachable=yes --error-exitcode=1 $(OUT_DIR)/$(OUTNAME) ./test/test1 -q
	! valgrind -q --leak-check=full --show-reachable=yes $(OUT_DIR)/$(OUTNAME) ajndhsy20fwnoc02nsuya0d3h -q
