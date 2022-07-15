#include "fisher.h"
#include <stdio.h>

int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer, int len) {
    int pos = 0;
    buffer[pos] = (uint8_t) frame->type;
    pos += 1;

    buffer[pos] = (uint8_t) frame->originator;
    pos += 1;

    buffer[pos] = (uint8_t) frame->sender;
    pos += 1;

    buffer[pos] = (uint8_t) frame->receiver;
    pos += 1;

    buffer[pos] = (uint8_t) frame->recipient;
    pos += 1;

    buffer[pos] = (uint8_t) frame->hops;
    pos += 1;

    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO:
            buffer[pos] = (uint8_t) frame->seq;
            break;

        default:
            printf("ooo ein anderes paket wurde geschickt yahelele!");
            break;
    }


    return pos;
}