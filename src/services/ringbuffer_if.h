#ifndef SRC_SERVICES_RINGBUFFER_IF_H
#define SRC_SERVICES_RINGBUFFER_IF_H

#include <stdint.h>

typedef struct {
	uint8_t *buffer;
	uint16_t length;
	volatile uint16_t readpos;
	volatile uint16_t writepos;
} ringbuffer_t;

extern void ringbuffer_init( ringbuffer_t *rb, void  *databuffer, uint16_t datalength );
extern uint16_t ringbuffer_get_free( ringbuffer_t *rb );
extern uint16_t ringbuffer_get_used( ringbuffer_t *rb );
extern uint16_t ringbuffer_get_length( ringbuffer_t *rb );

extern void ringbuffer_flush( ringbuffer_t *rb );

extern uint8_t ringbuffer_get_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);
extern uint8_t ringbuffer_put_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);

#endif
