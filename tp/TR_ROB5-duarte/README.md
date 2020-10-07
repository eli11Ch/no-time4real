# TP1 Real Time

*First Practical Work on the **real time computing**.*

*Statement : [*main-tp1.pdf*](main-tp1.pdf) (in french)*

## 2) Tricky conditions

*file : [condition.c](Ex2/condition.c).*

***Usage***
```s
$ make
$ ./ex2
```

### Question 2.1

In order to exacerbate the problem, we place a *sleep(1)* :

- Between the **sender thread** 's creation (`pthread_create(&id[0], NULL, sender, NULL)`) and the **receiver thread** 's creation (`pthread_create(&id[1], NULL, receiver, NULL)`) (inside the **main thread**) ; 
- Before the signal waiting trigger (`pthread_cond_wait(&c, &m)`) (inside the **receiver thread**).

The idea is to give time advance to the **sender thread** and delay the signal waiting trigger of the **receiver thread**.

### Question 2.2

To resolve the problem, we place :

- A `pthread_mutex_lock(&m)` before the `pthread_cond_signal(&c)` (inside the **sender thread**) ;
- A `pthread_mutex_unlock(&m)` after the `pthread_mutex_lock(&m)` (inside the **sender thread**).

As the mutex is firstly lock by the **main thread**, the **sender thread** need to wait until the mutex will be unlocked (by the signal waiting trigger of the **receiver thread**).
The **sender thread** does not need to keep the mutex lock so we decided to unlock it right after.

### Question 2.3

In order to launch 3 **receiver threads**, we change :

- The number of `pthread_t` from 2 to 4 (inside the **main thread**) ;
```c
    //pthread_t id[2]; // Initially
    pthread_t id[4]; // Question 2.3
```
- The **receiver thread** 's creation by a loop of **receiver threads** 's creation (inside the **main thread**) ;
```c
    //pthread_create(&id[1], NULL, receiver, NULL); // Initially
    for(int i = 0; i < 3; i = i + 1) { // Question 2.3
	    pthread_create(&id[i+1], NULL, receiver, NULL);
    }
```
- The **receiver thread** 's destruction by a loop of **receiver threads** 's destruction (inside the **main thread**) ;
```c
    //pthread_join(id[1], NULL); // Initially
    for(int i = 0; i < 3; i = i + 1) { // Question 2.3
	    pthread_join(id[i+1], NULL);
    }
```
- `pthread_cond_signal(&c)` by `pthread_cond_broadcast(&c)` (inside the **sender thread**).
```c
    //pthread_cond_signal(&c); // Initially
	pthread_cond_broadcast(&c); // Question 2.3
```

The three first modifications are obvious. The last is to send the signal to all the waiting thread and not only one.

To correctly unlock the three **receiver threads**, we use a semaphore :

- We include the library and a static semaphore ;
```c
#include <semaphore.h> // Question 2.3

static sem_t s; // Question 2.3
```

- We change the lock and unlock mutex by initializing and destroying the semaphore (inside the **main thread**) ;
```c
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
```

- We add a mutex lock and a semaphore post (inside the **receiver thread**) ;
```c
    pthread_mutex_lock(&m); // Question 2.3
    printf("%08lX: Begin waiting\n", pthread_self());
    sleep(1); // Question 2.1
    sem_post(&s); // Question 2.3
    pthread_cond_wait(&c, &m);
    sem_post(&s);
    printf("%08lX: Received signal. count=%d\n", pthread_self(), count);
    pthread_mutex_unlock(&m); // Question 2.3
```

- We add two loops in order to wait three semaphores post and to synchronise the sent and recieved signals (inside the **sender thread** and inside the **if loop of sender thread** ).
```c
	for(int i = 0; i < 3; i = i + 1) { // Question 2.3
		sem_wait(&s);
	}
```

While the **sender thread** waits for the semaphore post, each **receiver thread** will lock the mutex, post the semaphore and wait for the signal.

So the **sender thread** has to wait until all the **receiver threads** are ready to receive the signal. 



## 3) Safe Ressource

*files : [safe_ressource.c](Ex3/safe_ressource.c), [cir_buf.h](Ex3/cir_buf.h), [cir_buf.c](Ex3/cir_buf.c).*

***Usage***
```s
$ make
$ ./ex3
```
### Question 3.1

Here the implementations of `safe_send` and `safe_task` :

```c
int safe_send(char *data, size_t len) { // Question 3.1
	pthread_mutex_lock(&m);
	send(data, len);
	pthread_mutex_unlock(&m);
	return 0;
}
```

```c
void *safe_task(void *arg) { // Question 3.1
    char *buf = (char *) arg;

    safe_send(buf, SZ);
    return NULL;
}
```

### Question 3.2

For the same list of words :
```s
$ ./ex3
Aa1b[2B{3cC(4dD5EF)e}]6f
time task : 32.018 ms
ABCDEFabcdef[{()}]123456
time safe_task : 129.486 ms
```

### Question 3.3

The idea is to implemente a [Producer/Consumer Design Pattern](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem) with a [Circular Buffer](https://en.wikipedia.org/wiki/Circular_buffer).

- The goal of the **producer thread** is to writer word in the buffer ;
```c
void *producer_task(void *arg) { // Question 3.3
	char *buf = (char *) arg;
	pthread_mutex_lock(&m);
	for(size_t i = 0; i < SZ; i++) {
		cir_write(&cir_buf, buf[i]);
	}
	pthread_mutex_unlock(&m);
	return NULL;	
}
```

- The goal of the **consumer thread** is to read words in the buffer and send them ;
```c
void *consumer_task(void *arg) { // Question 3.3
	while(!cir_is_empty(&cir_buf)) {
		send(cir_read(&cir_buf),(size_t) 1);
	}
	return NULL;
}
```

In [cir_buf.h](Ex3/cir_buf.h), you can find a simple implementation of a circular buffer.

There is 2 interesting functions :

- `cir_write(cir_buf_t *c, char data)` is to write in the circular buffer. If the circular buffer is full, it will wait a *less signal* (generate when something is readed in the circular buffer) ;
```c
void cir_write(cir_buf_t *c, char data) {
	pthread_mutex_lock(&c->mutex);
	while(cir_is_full(c)) {
		pthread_cond_wait(&c->less, &c->mutex);
	}
	c->data[c->writer]=data;
	c->writer = (c->writer + 1)%c->size;
	c->count ++;
	pthread_cond_signal(&c->more);
	pthread_mutex_unlock(&c->mutex);
}
```

- `cir_read(cir_buf_t *c)` is to read in the circular buffer. If the circular buffer is empty, it will wait a *more signal* (generate when something is wrote in the circular buffer) ;
```c
char* cir_read(cir_buf_t *c) {
	pthread_mutex_lock(&c->mutex);
	while(cir_is_empty(c)) {
		pthread_cond_wait(&c->more, &c->mutex);
	}
	char* data = c->data + c->reader;
	c->reader = (c->reader + 1)%c->size;
	c->count --;
	pthread_cond_signal(&c->less);
	pthread_mutex_unlock(&c->mutex);
	return data;
}
```

The idea is to lock the circular buffer when it is used (for writing or reading). If the wished action is unavailable, the thread waits until the action is available.

## Related Documents

- [The Producer/Consumer Problem](https://docs.oracle.com/cd/E19455-01/806-5257/sync-31/index.html)
