#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

#define SZ 7

void *task(void *arg) {
    char *buf = (char *) arg;

    send(buf, SZ);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t id[4];
    int i;
    char msgs[4][SZ] = { "123456", "ABCDEF", "abcdef", "[{()}]" };

    for (i = 0; i < 4; i++)
	pthread_create(&id[i], NULL, task, msgs[i]);

    for (i = 0; i < 4; i++)
	pthread_join(id[i], NULL);

    printf("\n");
    return 0;
}
