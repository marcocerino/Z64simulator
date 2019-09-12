CC = gcc `pkg-config gtk+-3.0  --cflags --libs` -lm

main_test:main.c code.h code.c instruction.h instruction.c error_handler.h error_handler.c microcode.h microcode.c 
	$(CC) -o main_test main.c code.h code.c instruction.h instruction.c error_handler.h error_handler.c microcode.h microcode.c `pkg-config gtk+-3.0  --cflags --libs` -lm