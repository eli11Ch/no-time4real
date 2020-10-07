#include "cir_buf.h"

void cir_init(cir_buf_t *c, size_t size) {
	c->size = size;
	c->count = 0;
	c->data = (char*) malloc(sizeof(char)*c->size);
	c->reader = 0;
	c->writer = 0;
	pthread_mutex_init(&c->mutex, NULL);
	pthread_cond_init(&c->c_more, NULL);
	pthread_cond_init(&c->c_less, NULL);
	sem_init(&c->s_empty, 0, size);
	sem_init(&c->s_fill, 0, 0);
}

void cir_free(cir_buf_t *c) {
	free(c->data);
	pthread_mutex_destroy(&c->mutex);
	pthread_cond_destroy(&c->c_more);
	pthread_cond_destroy(&c->c_less);
	sem_destroy(&c->s_empty);
	sem_destroy(&c->s_fill);
}

void cir_write(cir_buf_t *c, char data) {
	pthread_mutex_lock(&c->mutex);
	while(cir_is_full(c)) {
		pthread_cond_wait(&c->c_less, &c->mutex);
	}
	c->data[c->writer]=data;
	c->writer = (c->writer + 1)%c->size;
	c->count ++;
	pthread_cond_signal(&c->c_more);
	pthread_mutex_unlock(&c->mutex);
}

char cir_read(cir_buf_t *c) {
	pthread_mutex_lock(&c->mutex);
	while(cir_is_empty(c)) {
		pthread_cond_wait(&c->c_more, &c->mutex);
	}
	char data = c->data[c->reader];
	c->reader = (c->reader + 1)%c->size;
	c->count --;
	pthread_cond_signal(&c->c_less);
	pthread_mutex_unlock(&c->mutex);
	return data;
}

void cir_write_s(cir_buf_t *c, char data) {
	sem_wait(&c->s_empty);
	pthread_mutex_lock(&c->mutex);
	c->data[c->writer]=data;
	c->writer = (c->writer + 1)%c->size;
	c->count ++;
	pthread_mutex_unlock(&c->mutex);
	sem_post(&c->s_fill);
}

char cir_read_s(cir_buf_t *c) {
	sem_wait(&c->s_fill);
	pthread_mutex_lock(&c->mutex);
	char data = c->data[c->reader];
	c->reader = (c->reader + 1)%c->size;
	c->count --;
	pthread_mutex_unlock(&c->mutex);
	sem_post(&c->s_empty);
	return data;
}

int cir_is_empty(cir_buf_t *c) {
	return (c->count<=0);
}

int cir_is_full(cir_buf_t *c) {
	return (c->count>=c->size);
}