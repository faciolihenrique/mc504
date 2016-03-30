CC = gcc
CFLAGS = -g -pthread -lncurses

PROGRAMS = fme fme_animation

all: $(PROGRAMS)

clean:
	rm -f *.o *~ $(PROGRAMS)
