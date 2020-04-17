CC=gcc
CFLAGS=-I.
DEPS = comp_rscoding.h arithmetic.h utility.h comp_arithmetic.h poly_comp_arithmetic.h
OBJ = main.o comp_rscoding.o arithmetic.o utility.o comp_arithmetic.o poly_comp_arithmetic.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o main
