#include <stdint.h>

int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer);
int fisher_deserialize( struct fisher_frame *frame, uint8_t* buffer);