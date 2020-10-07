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
#include <semaphore.h> // Question 2.3

static sem_t s; // Question 2.3
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static int count = 0;

void *sender(void *arg) {
	for(int i = 0; i < 3; i = i + 1) { // Question 2.3
		sem_wait(&s);
	}
    int i;
    for (i = 0; i < 10; i++) {
    	count++;
    	printf("%08lX: Increment count: count=%d\n", pthread_self(), count);
    	if (count == 3) {
		pthread_mutex_lock(&m); // Question 2.2
  		pthread_mutex_unlock(&m); // Question 2.2
    	    //pthread_cond_signal(&c); // Initially
		pthread_cond_broadcast(&c); // Question 2.3
		for(int i = 0; i < 3; i = i + 1) { // Question 2.3
			sem_wait(&s);
		}
        	printf("%08lX: Sent signal. count=%d\n", pthread_self(), count);
        }
    }
    return NULL;
}

void *receiver(void *arg) {
    pthread_mutex_lock(&m); // Question 2.3
    printf("%08lX: Begin waiting\n", pthread_self());
    sleep(1); // Question 2.1
    sem_post(&s); // Question 2.3
    pthread_cond_wait(&c, &m);
    sem_post(&s); // Question 2.3
    printf("%08lX: Received signal. count=%d\n", pthread_self(), count);
    pthread_mutex_unlock(&m); // Question 2.3
    return NULL;
}


int main(int argc, char **argv) {
    //pthread_t id[2]; // Initially
    pthread_t id[4]; // Question 2.3
    
    //pthread_mutex_lock(&m); // Initially
   	sem_init(&s, 0, 0); // Question 2.3

    pthread_create(&id[0], NULL, sender, NULL);
    sleep(1); // Question 2.1
    //pthread_create(&id[1], NULL, receiver, NULL); // Initially
    for(int i = 0; i < 3; i = i + 1) { // Question 2.3
	    pthread_create(&id[i+1], NULL, receiver, NULL);
    }

    pthread_join(id[0], NULL);
    //pthread_join(id[1], NULL); // Initially
    for(int i = 0; i < 3; i = i + 1) { // Question 2.3
	    pthread_join(id[i+1], NULL);
    }
    //pthread_mutex_unlock(&m); // Initially
	sem_destroy(&s); // Question 2.3
    return 0;
}
