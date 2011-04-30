INCS = -I.
LIBS = -lc -L/usr/lib

SDL_LIBS = -L/usr/lib -lSDL -lpthread -lm -ldl -lasound -lm -ldl -lpthread -lesd -lpulse-simple -lpulse -L/usr/lib -ldirectfb -lfusion -ldirect -lpthread -laa -L/usr/lib -lcaca -lpthread

CFLAGS = -g -std=c99 -pedantic -Wall -Os ${INCS} -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
LDFLAGS = ${LIBS} ${SDL_LIBS}