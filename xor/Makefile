# Setting the compiler and the default linker program
CC = gcc
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS = `pkg-config --cflags sdl` -MMD
# main compilation options
CFLAGS = -g -Wall -Wextra -Werror -std=c99 -O3

# The -W are for the errors
# -std=c99 is to define the version of the code
# -O3 is an optimization function (https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

#Linker options (probably always empty)
LDFLAGS = 
# libs and path for linker
LDLIBS = `pkg-config --libs sdl` -lm

all: xor

xor:	xor.o 

clean:
	${RM} *.o
	${RM} *.d
	${RM} *.bmp
	${RM} xor
