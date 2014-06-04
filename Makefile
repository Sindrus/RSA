CC = gcc
TOP = .
CFLAGS = -g -std=c99
COMPILE = $(CC) $(CFLAGS)
PROGRAMS = clean testmod rsa

all: $(PROGRAMS)

testmod:
	$(COMPILE) -o testmod test_mod_inv.c -lgmp

rsa:
	$(COMPILE) -o rsa RSA.c -lgmp

clean:
	rm rsa
	rm testmod
