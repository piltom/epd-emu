example: src/example.c src/epd_emu_window.c src/epd_emu.c
	gcc -Wall -Werror -lm -lSDL2 src/epd_emu.c src/epd_emu_window.c src/example.c -o example.o

clean:
	rm example.o