# TP1 Real Time

*First Practical Work on the **real time computing**.*

*Statement : [*main-tp1.pdf*](main-tp1.pdf) (in french)*

## 2) Tricky conditions

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

---

To correctly unlocks the 3 **receiver threads**, we use a semaphore :

- We include the library and a static semaphore ;
```c
#include <semaphore.h> // Question 2.3

static sem_t s; // Question 2.3
```

- We change the lock and unlock mutex by initialyze and destroy the semaphore (inside the **main thread**) ;
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
    printf("%08lX: Received signal. count=%d\n", pthread_self(), count);
    pthread_mutex_unlock(&m); // Question 2.3
```

- We add a loop in order to wait three semaphores post (inside the **sender thread**).
```c
	for(int i = 0; i < 3; i = i + 1) { // Question 2.3
		sem_wait(&s);
	}
```

While the **sender thread** waits for the semaphore post, each **receiver thread** will lock the mutex, post the semaphore and wait for the signal.

So the **sender thread** has to wait until all the **receiver threads** are ready to receive the signal. 