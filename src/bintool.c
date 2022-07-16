#include "fisher.h"
#include <stdio.h>
#include <string.h>

int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer) {
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
        case FISHER_FRAME_TYPE_DATA :
            buffer[pos] = (uint8_t) frame->length;
            pos++;
            memcpy(buffer + pos, frame->content, frame->length);
            pos += frame->length;
            break;

        default:
            printf("ooo ein anderes paket wurde geschickt yahelele!");
            break;
    }

    return pos;
}

int fisher_deserialize(struct fisher_frame *frame, uint8_t* buffer) {

    int pos = 0;
    frame->type = buffer[pos];
    pos += 1;

    frame->originator = buffer[pos];
    pos += 1;

    frame->sender = buffer[pos];
    pos += 1;

    frame->receiver = buffer[pos];
    pos += 1;

    frame->recipient = buffer[pos];
    pos += 1;

    frame->hops = buffer[pos];
    pos += 1;

    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO :
            frame->seq = buffer[pos];
            break;
        case FISHER_FRAME_TYPE_DATA:
                frame->length = buffer[pos];
                memcpy(frame->content, buffer + pos, frame->length);
                break;
        default:
            printf("not implementet yet!");
            break;
    }
    return pos;
}


