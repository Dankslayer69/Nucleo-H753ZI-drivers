CC ?= gcc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Iinclude
SRC = icm42686.c icm42686_cmsis.c
OBJ = $(SRC:.c=.o)
LIB = libicm42686.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)

.PHONY: all clean
