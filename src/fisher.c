//
// Created by Jurek on 21.06.22.
//
#include "fisher.h"
#include <stdio.h>
#include <string.h>

#define HELLO_EVEY_TICKS 100

Status fisher_init(struct fisher_boat *boat, Address addr){
    // init all with 0, create new routing table etc
    boat->addr = addr;
    boat->to_be_sent_read = 0;
    boat->to_be_sent_write = 0;
    boat->to_be_sent_count = 0;

    for (int i = 0; i < ROUTINGSIZE; ++i) {
        boat->routing_tabele[i] = NULL;
    }

    boat->tick =0;
    boat->hello_evey_tick = HELLO_EVEY_TICKS;
    boat->hello_seq = 0;
    return OK;
}

// gets executed evey loop, generate HELLO frames
// #pre boat in old state
// #post if algorithm decides to send hello messages,
Status fisher_tick(struct fisher_boat *boat) {
    // TODO
    // compare if time has passed to send ogm2 msg
    // generate ogm2 msg
    // send
    if (boat->tick % boat->hello_evey_tick == 0) {
        fisher_frame_generate_hello(boat);
    }

    boat->tick++;
    return OK;
}

// generates data frames
Status fisher_packet_generate(struct fisher_boat *boat /* TODO */) {

}
Status fisher_packet_read(struct fisher_boat* boat, struct fisher_frame *frame) {
    // TODO
    // check if it is a ogm msg and forward
    if (frame == NULL) {
        printf("Error! processing empty frame!");
        return ERR;
    }
    // drop packet if receiver does not match own address/broadcast
    if (frame->receiver != boat->addr && frame->receiver != BROADCAST) return OK;

    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO:
            printf("Recieved HELLO PKG... Forwarding...\n");
            if (frame->ttl == 0) {
                // drop packet
                printf("TTL == 0, dropping pkg\n");
                break;
            }
            // retransmit
            struct fisher_frame *frame_out = fisher_add_frame(boat);
            memcpy(frame_out, frame, sizeof(struct fisher_frame));
            frame_out->ttl--;
            frame_out->sender = boat->addr;
            break;
    }
}

struct fisher_frame * fisher_frame_get_to_be_sent(struct fisher_boat *boat) {
    if (boat->to_be_sent_count == 0) return NULL;
    boat->to_be_sent_count--;
    struct fisher_frame *ret = &boat->to_be_sent[boat->to_be_sent_read];
    boat->to_be_sent_read = (boat->to_be_sent_read + 1) % MAX_FRAME_BUFFER_SIZE;
    return ret;
}

/*
 * Adds a frame to the to be sended queue returns null if queue is full
 */
struct fisher_frame * fisher_add_frame(struct fisher_boat *boat) {
    if (boat->to_be_sent_count == MAX_FRAME_BUFFER_SIZE) return NULL;
    struct fisher_frame *ret = &(boat->to_be_sent[boat->to_be_sent_write]);
    boat->to_be_sent_write = (boat->to_be_sent_write + 1) % MAX_FRAME_BUFFER_SIZE;
    boat->to_be_sent_count++;
    return ret;
}

Status fisher_frame_generate_hello(struct fisher_boat *boat) {
    if (boat->addr == 123) {
        printf("Generating hello packet (%d)...\n", boat->hello_seq);
    }

    struct fisher_frame *pkg = fisher_add_frame(boat);
    boat->hello_seq++;
    if (pkg == NULL) {
        printf("ERROR buffer full!\n");
        return ERR;
    }
    pkg->type = FISHER_FRAME_TYPE_HELLO;
    pkg->originator = boat->addr;
    pkg->sender = boat->addr;
    pkg->recipient = BROADCAST;
    pkg->receiver = BROADCAST;
    pkg->seq = boat->hello_seq - 1;
    pkg->ttl = MAXIMUM_TTL;
    return OK;
}
/*
 * debug frame
 */
void fisher_frame_print(struct fisher_frame *frame) {
    printf("----\nFisher packet   type:");
    switch (frame->type) {
        case (FISHER_FRAME_TYPE_HELLO):
            printf("HELLO");
            printf(" seq:\t%d\tttl: %d", frame->seq, frame->ttl);
            printf("\n%d\t%d\t -> \t%d --> %d\n", frame->originator, frame->sender, frame->receiver, frame->recipient);
            break;
        default:
            printf("UNKNOWN");
    }
    printf("\n----\n");
}