#include "cir_buf.h"

void cir_init(cir_buf_t *c, size_t size) {
	c->size = size;
	c->data = (char*) malloc(sizeof(char)*c->size);
	c->reader = 0;
	c->writer = 0;
}

void cir_free(cir_buf_t *c) {
	free(c->data);
}

void cir_write(cir_buf_t *c, char data) {
	c->data[c->writer]=data;
	c->writer = (c->writer + 1)%c->size;
}

char* cir_read(cir_buf_t *c) {
	char* data = c->data + c->reader;
	c->reader = (c->reader + 1)%c->size;
	return data;
}
