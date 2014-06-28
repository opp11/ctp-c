SRC_DIR=src
OUT_DIR=bin
OBJ_DIR=$(OUT_DIR)/obj

OUTNAME=ctp

CC=gcc
CC_FLAGS=-Wall -Wextra -c
LC_FLAGS=-I$(SRC_DIR) -Wall -Wextra 

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LC_FLAGS) -o $(OUT_DIR)/$(OUTNAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean: objclean
	rm -rf $(OUT_DIR)/$(OUTNAME)

objclean:
	rm -rf $(wildcard $(OBJ_DIR)/*.o)
