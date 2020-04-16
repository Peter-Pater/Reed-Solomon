CC=gcc
CFLAGS=-I.
DEPS = stdrscoding.h arithmetic.h utility.h comprs_arithmetic.h
OBJ = main.o stdrscoding.o arithmetic.o utility.o comprs_arithmetic.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o main
