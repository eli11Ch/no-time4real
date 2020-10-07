#ifndef CIR_BUF_H
#define CIR_BUF_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct cir_buf {
	char *data;
	size_t size;
	size_t count;
	size_t reader; // read the buffer
	size_t writer; // fill the buffer
	pthread_mutex_t mutex;
	pthread_cond_t c_more;
	pthread_cond_t c_less;
	sem_t s_empty;
	sem_t s_fill;
} cir_buf_t;


/**
 * cir_init
 * 
 * Initialise the circular buffer
 * @param cir_buf_t* circular buffer pointer
 * @param size_t the size of the buffer
 */
void cir_init(cir_buf_t *c, size_t size);

/**
 * Cir_free
 * 
 * Deallocate the pointer memory
 * @param cir_buf_t* circular buffer pointer
 */
void cir_free(cir_buf_t *c);

/**
 * cir_write
 *
 * Write on the buffer
 * @param cir_buf_t* circular buffer pointer
 * @param char data to write
 */
void cir_write(cir_buf_t *c, char data);

/**
 * cir_read
 *
 * Read the buffer content
 * @param cir_buf_t* circular buffer pointer
 * @return char the character pointer by the reader
 */
char cir_read(cir_buf_t *c);

/**
 * cir_write_s
 *
 * Write on the buffer with semaphore
 * @param cir_buf_t* circular buffer pointer
 * @param char data to write
 */
void cir_write_s(cir_buf_t *c, char data);

/**
 * cir_read_s
 *
 * Read the buffer content with semaphore
 * @param cir_buf_t* circular buffer pointer
 * @return char the character pointer by the reader
 */
char cir_read_s(cir_buf_t *c);

/**
 * cir_is_empty
 * 
 * predicate for empty state
 * @param cir_buf_t* circular buffer pointer
 * @return int 1 if is empty, 0 else
 */
int cir_is_empty(cir_buf_t *c);

/**
 * cir_is_full
 * 
 * predicate for full state
 * @param cir_buf_t* circular buffer pointer
 * @return int 1 if is full, 0 else
 */
int cir_is_full(cir_buf_t *c);

#endif // CIR_BUF_H
