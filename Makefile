include config.mk

SRC = drawlib.c
OBJ = ${SRC:.c=.o}

SKETCHES = sketch_skel
SKOBJ = ${SKETCHES:=.o}

all: ${SKETCHES}

${SKETCHES}: ${OBJ} ${SKOBJ}
	${CC} ${CFLAGS} -o $@ $@.o ${OBJ} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk


clean:
	@echo cleaning
	rm ${OBJ} ${SKETCHES} ${SKOBJ}

.PHONY: all options clean 

