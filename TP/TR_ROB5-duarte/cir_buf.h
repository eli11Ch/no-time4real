#ifndef CIR_BUF_H
#define CIR_BUF_H

#include<stdlib.h>
#include<pthread.h>

typedef struct cir_buf {
	char *data;
	size_t size;
	size_t reader;
	size_t writer; // fill the buffer
	pthread_mutex_t	m = PTHREAD_MUTEX_INITIALIZER;
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
 * @output char the character points by the reader
 */
char* cir_read(cir_buf_t *c);

#endif // CIR_BUF_H
