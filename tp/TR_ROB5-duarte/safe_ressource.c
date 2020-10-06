#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cir_buf.h"

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // Question 3.1

static cir_buf_t cir_buf;

void send_char(char data) {
    double time = (double) random() / RAND_MAX * 8;
    struct timespec t;
    t.tv_sec = time / 1000;
    t.tv_nsec = (int) (time * 1000000) % 1000000000;
    nanosleep(&t, NULL);
    write(1, &data, 1);
}

void send(char *data, size_t len) {
    unsigned i;
 
    for (i = 0; i < len; i++)
	send_char(data[i]);
}

int safe_send(char *data, size_t len) { // Question 3.1
	pthread_mutex_lock(&m);
	send(data, len);
	pthread_mutex_unlock(&m);
	return 0;
}

#define SZ 7

void *task(void *arg) {
    char *buf = (char *) arg;

    safe_send(buf, SZ); // Question 3.1
    return NULL;
}

void *producer_task(void *arg) { // Question 3.3
	char *buf = (char *) arg;
	pthread_mutex_lock(&m);
	for(size_t i = 0; i < SZ; i++) {
		cir_write(&cir_buf, buf[i]);
	}
	pthread_mutex_unlock(&m);
	return NULL;	
}

void *consumer_task(void *arg) { // Question 3.3
	while(!cir_is_empty(&cir_buf)) {
		send(cir_read(&cir_buf),(size_t) 1);
	}
	return NULL;
}

int main(void) {
    cir_init(&cir_buf, SZ-2); // Question 3.3
    pthread_t id[5]; // Question 3.3
    int i;
    char msgs[4][SZ] = { "123456", "ABCDEF", "abcdef", "[{()}]" };
	size_t size = 4*SZ;
    for (i = 0; i < 4; i++) {
		//pthread_create(&id[i], NULL, task, msgs[i]);
		pthread_create(&id[i], NULL, producer_task, msgs[i]); // Question 3.3
	}
	pthread_create(&id[4], NULL, consumer_task, (void *) size); // Question 3.3

	pthread_join(id[4], NULL);
    for (i = 0; i < 4; i++) {
		pthread_join(id[i], NULL);
	}
	cir_free(&cir_buf);
    printf("\n");
    return 0;
}
