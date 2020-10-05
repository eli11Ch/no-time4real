FLAGS = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv) -lm

all: oc_camera.c
	gcc $(FLAGS) oc_camera.c -o test $(LIBS)
