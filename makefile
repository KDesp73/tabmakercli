all: tabmakercli

tabmakercli: tabmakercli.c
	gcc -ggdb -lncurses tabmakercli.c -o tabmakercli
