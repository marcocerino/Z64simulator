CC = gcc 
CFLAGS = `pkg-config gtk+-3.0  --cflags --libs` -lm
MAIN = main.c code.h code.c instruction.h instruction.c error_handler.h error_handler.c microcode.h microcode.c useful.h useful.c

all:  test_file Z64Simulator ui


test_file: test_file.c
	$(CC) -o test_file test_file.c $(CFLAGS)

Z64Simulator: $(MAIN)
	$(CC) -o Z64Simulator $(MAIN) $(CFLAGS)

ui: ui.c useful.h useful.c
	$(CC)-o ui ui.c useful.h useful.c $(CFLAGS) 
