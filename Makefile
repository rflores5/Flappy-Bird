all: final

final: final.c 
	gcc final.c gfx3.c -lm -lX11 -o final

clean: 
	rm final
