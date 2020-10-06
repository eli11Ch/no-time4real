CC= gcc
CFLAGS= -Wall $(shell xeno-config --xeno-cflags)
LDFLAGS= -Wall $(shell xeno-config --xeno-ldflags --native) -lpthread -lrt
EXEC= ex2 ex3
#SRC= $(wildcard *.c)
#OBJ= $(SRC:.c=.o)

all: $(EXEC)

ex2: condition.o
	$(CC) -o $@ $^ $(LDFLAGS)

ex3: safe_ressource.o cir_buf.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
