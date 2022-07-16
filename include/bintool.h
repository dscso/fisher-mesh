#include <stdint.h>
int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer, int len_in_byte);
int fisher_deserialize( struct fisher_frame *frame, uint8_t* buffer,int  len_in_byte);