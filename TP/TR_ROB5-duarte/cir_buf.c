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
	pthread_mutex_lock(&(c->m));
	c->data[c->writer]=data;
	if(c->writer==c->reader) {
		c->reader = (c->reader + 1)%c->size;
	}
	c->writer = (c->writer + 1)%c->size;
	pthread_mutex_unlock(&(c->m));	
}

char* cir_read(cir_buf_t *c) {
	pthread_mutex_lock(&(c->m));
	char* data = c->data + c->reader;
	c->reader = (c->reader + 1)%c->size;
	pthread_mutex_unlock(&(c->m));
	return data;
}
