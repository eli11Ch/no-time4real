/*
 * Creation date: 2010-01-03 11:22:08+01:00
 * Licence: GPL
 * Main authors:
 *   - Jérôme Pouiller <jerome@sysmic.org>
 *
 * Sample use of conditions.
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static int count = 0;

void *sender(void *arg) {
    int i;

    for (i = 0; i < 10; i++) {
	count++;
	printf("%08lX: Increment count: count=%d\n", pthread_self(), count);
	if (count == 3) {
	    pthread_cond_signal(&c);
    	    printf("%08lX: Sent signal. count=%d\n", pthread_self(), count);
	}
    }
    return NULL;
}

void *receiver(void *arg) {
    printf("%08lX: Begin waiting\n", pthread_self());
    pthread_cond_wait(&c, &m);
    printf("%08lX: Received signal. count=%d\n", pthread_self(), count);
    return NULL;
}


int main(int argc, char **argv) {
    pthread_t id[2];

    pthread_mutex_lock(&m);
    pthread_create(&id[0], NULL, sender, NULL);
    pthread_create(&id[1], NULL, receiver, NULL);

    pthread_join(id[0], NULL);
    pthread_join(id[1], NULL);
    pthread_mutex_unlock(&m);

    return 0;
}
