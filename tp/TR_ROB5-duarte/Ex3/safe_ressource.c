#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // Question 3.2
#include "cir_buf.h" // Question 3.3

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // Question 3.1

static cir_buf_t cir_buf;

/**
 * duration
 *
 * calculates the time difference in ms
 * @param start struct timespec* time beginning
 * @param end struct timespec* time ending
 * @return double time difference in ms
 */
double duration(struct timespec* start, struct timespec* end) { // Question 3.2
	double d = difftime(end->tv_sec, start->tv_sec) * 1000;
	d += ((double) (end->tv_nsec - start->tv_nsec)) / 1000000.0;
	return d;
}

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

    send(buf, SZ);
    return NULL;
}

void *safe_task(void *arg) { // Question 3.1
    char *buf = (char *) arg;

    safe_send(buf, SZ);
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
		send_char(cir_read(&cir_buf));
	}
	return NULL;
}

void *producer_task_s(void *arg) { // Question 3.3
	char *buf = (char *) arg;
	pthread_mutex_lock(&m);
	for(size_t i = 0; i < SZ; i++) {
		cir_write_s(&cir_buf, buf[i]);
	}
	pthread_mutex_unlock(&m);
	return NULL;	
}

void *consumer_task_s(void *arg) { // Question 3.3
	while(!cir_is_empty(&cir_buf)) {
		send_char(cir_read_s(&cir_buf));
	}
	return NULL;
}

int main(int argc, char **argv) {
    pthread_t id[5];
    int i;
    char msgs[4][SZ] = { "123456", "ABCDEF", "abcdef", "[{()}]" };

	struct timespec start, end; // Question 3.2

    clock_gettime(CLOCK_REALTIME, &start); // Question 3.2
    for (i = 0; i < 4; i++) {
    	pthread_create(&id[i], NULL, task, msgs[i]);
    }

    for (i = 0; i < 4; i++) {
    	pthread_join(id[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end); // Question 3.2
    printf("\n");

	printf("time task : %.3lf ms\n", duration(&start, &end)); // Question 3.2
    
    clock_gettime(CLOCK_REALTIME, &start); // Question 3.2
    for (i = 0; i < 4; i++) { // Question 3.1
	    pthread_create(&id[i], NULL, safe_task, msgs[i]);
    }
    
    for (i = 0; i < 4; i++) { // Question 3.1
    	pthread_join(id[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end); // Question 3.2
    printf("\n");

	printf("time safe_task : %.3lf ms\n", duration(&start, &end)); // Question 3.2

    cir_init(&cir_buf, SZ*4); // Question 3.3
    clock_gettime(CLOCK_REALTIME, &start);
    for (i = 0; i < 4; i++) {
		pthread_create(&id[i], NULL, producer_task, msgs[i]); // Question 3.3
	}
	pthread_create(&id[4], NULL, consumer_task, NULL);

	pthread_join(id[4], NULL);
    for (i = 0; i < 4; i++) {
		pthread_join(id[i], NULL);
	}
    clock_gettime(CLOCK_REALTIME, &end);
    printf("\n");

	printf("time prod_consum_task with signal : %.3lf ms\n", duration(&start, &end)); // Question 3.2

	cir_free(&cir_buf);

    cir_init(&cir_buf, SZ*4); // Question 3.3
    clock_gettime(CLOCK_REALTIME, &start);
    for (i = 0; i < 4; i++) {
		pthread_create(&id[i], NULL, producer_task_s, msgs[i]); // Question 3.3
	}
	pthread_create(&id[4], NULL, consumer_task_s, NULL);

	pthread_join(id[4], NULL);
    for (i = 0; i < 4; i++) {
		pthread_join(id[i], NULL);
	}
    clock_gettime(CLOCK_REALTIME, &end);
    printf("\n");

	printf("time prod_consum_task with semaphore : %.3lf ms\n", duration(&start, &end)); // Question 3.2

	cir_free(&cir_buf);
    return 0;
}