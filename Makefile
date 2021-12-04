# Setting the compiler and the default linker program
CC = gcc
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS = `pkg-config --cflags sdl gtk+-3.0` -MMD
# main compilation options
CFLAGS = -g -Wall -Wextra -std=c99 -O3 -fsanitize=address

# The -W are for the errors
# -std=c99 is to define the version of the code
# -O3 is an optimization function (https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

#Linker options (probably always empty)
LDFLAGS = -fsanitize=address -ldl
# libs and path for linker
LDLIBS = `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lSDL_gfx -lm

SRC = $(wildcard image_preprocessing/*.c) $(wildcard split/*.o) $(wildcard rect_reconstruction/*.c) ai/sdltoarray.c ai/neuralnetwork.c  solver/solver.o main.o
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: main

main: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	${RM} *.o
	${RM} *.d
	${RM} *.bmp
	${RM} main
	${RM} split/*.o
	${RM} split/*.d
	${RM} image_preprocessing/*.o
	${RM} image_preprocessing/*.d
	${RM} rect_reconstruction/*.o
	${RM} rect_reconstruction/*.d
	${RM} ai/*.o
	${RM} ai/*.d
	${RM} solver/*.o
	${RM} solver/*.d
	${RM} *.result
	${RM} grid_00
