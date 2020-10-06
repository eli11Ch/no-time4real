CC= gcc
CFLAGS= -Wall #$(shell xeno-config --xeno-cflags)
LDFLAGS= -Wall -lpthread -lrt #$(shell xeno-config --xeno-ldflags --native)
EXEC= ex2 ex3
SRC= $(wildcard */*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

ex2: Ex2/condition.o
	$(CC) -o $@ $^ $(LDFLAGS)

ex3: Ex3/safe_ressource.o Ex3/cir_buf.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f $(EXEC)
