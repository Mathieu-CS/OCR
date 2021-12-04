# Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS= -fsanitize=address -Wl,-V
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lSDL_gfx -lm

SRC = main.c
OBJ = $(SRC: .c=.o)
DEP = $(SRC:.c=.d)

all: main

main: $(OBJ)

clean-output:
	${RM} ./**/*.bmp

clean:
	${RM} main.[do] 
	${RM} ./**/*.o
	${RM} ./**/*.d
	${RM} main
# END
