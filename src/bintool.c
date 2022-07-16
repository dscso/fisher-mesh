#include "fisher.h"
#include <stdio.h>



int fisher_serialize(struct fisher_frame *frame, uint8_t* buffer, int len_in_byte) {
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
        case FISHER_FRAME_TYPE_HELLO :
            buffer[pos] = (uint8_t) frame->seq;
            break;

/* not testsed
 * case FISHER_FRAME_TYPE_DATA :
            buffer[pos] = frame ->length;
            pos++;
            char *data = frame-> content;
            while (pos < frame->length) {
                buffer[pos] = data;
                data = data+1;
                pos++;

            break;
*/
        default:
            printf("ooo ein anderes paket wurde geschickt yahelele!");
            break;
    }

    return pos;
}

int fisher_deserialize( struct fisher_frame *frame, uint8_t* buffer, int len){

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
  /*      case FISHER_FRAME_TYPE_DATA
   *        frame -> length = buff[pos]
            int i = 0;
            pos++;
            while (pos < frame->length) {
                frame->content+i = buffer[pos];
                i ++;
                pos++;
            }

            break;
  */
        default:
            printf("not implementet yet!");
            break;
    }

    return pos;
}


