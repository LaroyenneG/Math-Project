MAINSRC= $(wildcard pend*.c)
LIBSRC= $(wildcard lib*.c)

EXEC= $(patsubst %.c,%,$(MAINSRC))
MAINOBJ= $(MAINSRC:.c=.o)
LIBH=$(LIBSRC:.c=.h)
LIBOBJ=$(LIBSRC:.c=.o)

# Options possibles : -DDEBUG -DPEND_SIMPLE -DCHAR_FIXE
# -DIR1 … -DIR3 ou -DAS1 … -DAS3
CFLAGS= -DIR1  -DDEBUG
LDFLAGS= -lm

.PHONY: clean

all: $(EXEC)

$(EXEC) : % : %.o $(LIBOBJ)
	gcc -o $@ $^ $(LDFLAGS)

$(MAINOBJ) : %.o : %.c $(LIBH)
	gcc -c -o $@ $(CFLAGS) $<

$(LIBOBJ) : %.o : %.c %.h
	gcc -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o $(EXEC)