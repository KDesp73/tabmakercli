all: tabmakercli

tabmakercli: tabmakercli.c
	gcc -g -lncurses tabmakercli.c -o tabmakercli
