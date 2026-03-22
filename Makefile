CC := cc
CC_ARGS := -std=c89 -Wall -Werror -O3

renderer: renderer.c
	$(CC) $(CC_ARGS) -o renderer renderer.c