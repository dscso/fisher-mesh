#include "fisher.h"
#include <stdio.h>
#include <string.h>

int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer) {
    int pos = 0;
    buffer[pos++] = (uint8_t) frame->type;

    buffer[pos++] = (uint8_t) frame->originator;

    buffer[pos++] = (uint8_t) frame->sender;

    buffer[pos++] = (uint8_t) frame->receiver;

    buffer[pos++] = (uint8_t) frame->recipient;

    buffer[pos++] = (uint8_t) frame->hops;

    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO:
            buffer[pos++] = (uint8_t) frame->seq;
            break;
        case FISHER_FRAME_TYPE_DATA :
            buffer[pos++] = (uint8_t) frame->length;

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
    frame->type = buffer[pos++];

    frame->originator = buffer[pos++];

    frame->sender = buffer[pos++];

    frame->receiver = buffer[pos++];

    frame->recipient = buffer[pos++];

    frame->hops = buffer[pos++];

    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO :
            frame->seq = buffer[pos++];
            break;
        case FISHER_FRAME_TYPE_DATA:
                frame->length = buffer[pos++];
                memcpy(frame->content, buffer + pos, frame->length);
                break;
        default:
            printf("not implementet yet!");
            break;
    }
    return pos;
}


