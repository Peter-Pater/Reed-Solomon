CC=gcc
CFLAGS=-I.
DEPS = arithmetic.h utility.h
OBJ = main.o arithmetic.o utility.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o main
