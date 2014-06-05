CC = gcc
TOP = .
CFLAGS = -g -std=c99
COMPILE = $(CC) $(CFLAGS)
PROGRAMS = testmod rsa

all: $(PROGRAMS)

testmod:
	$(COMPILE) -o testmod test_mod_inv.c -lgmp

rsa:
	$(COMPILE) -o rsa RSA.c -lgmp -lm

clean:
	rm rsa
	rm testmod
