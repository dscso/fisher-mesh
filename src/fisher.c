#include <stdio.h>
#include <string.h>

#include "fisher.h"
#define DEBUG printf("[%d]\t\t", boat->addr); printf

#define HELLO_EVEY_TICKS 10
#define TIMEOUT_TICKS    HELLO_EVEY_TICKS * 10
/**
 * boat constructor
 * @param boat context of the boat (either on stack or on heap)
 * @param addr address of the boat
 * @return status
 */
Status fisher_init(struct fisher_boat *boat, Address addr){
    // init all with 0, create new routing table etc
    boat->addr = addr;
    boat->to_be_sent_read = 0;
    boat->to_be_sent_write = 0;
    boat->to_be_sent_count = 0;

    fisher_route_init(boat);

    boat->tick = 0;
    boat->hello_evey_tick = HELLO_EVEY_TICKS;
    boat->hello_seq = 0;
    return OK;
}

//
// #pre boat in old state
// #post if algorithm decides to send hello messages,
/**
 * gets executed evey loop, generate HELLO frames
 * @param boat
 * @return status
 */
Status fisher_tick(struct fisher_boat *boat) {
    // TODO
    // compare if time has passed to send ogm2 msg
    // generate ogm2 msg
    // send
    if (boat->tick % boat->hello_evey_tick == 0) {
        fisher_frame_generate_hello(boat);
    }
    boat->tick++;
    // clean up routes
    for (int i = 0; i < sizeof(boat->routes) / sizeof(struct fisher_route); i++) {
        struct fisher_route *route = &(boat->routes[i]);

        if (!route->active) continue;
        if (route->last_update + TIMEOUT_TICKS < boat->tick) {
            route->active = false;
            DEBUG("Removing route to %d due to timeout!\n", route->node_address);
        }
    }
    return OK;
}

/**
 * generates data frames
 * @param boat
 * @param recipient
 * @param data
 * @param len
 * @return status
 * registers a new frame that should be sent and puts it in the to be sent buffer
 */
Status fisher_packet_generate(struct fisher_boat *boat, Address recipient, char *data, int len) {
    struct fisher_route *route = fisher_route_get(boat, recipient);
    if (route == NULL) {
        printf("Sorry, host not found! Could not send to %d!\n", recipient);
        return HOST_NOT_FOUND;
    }
    struct fisher_frame *pkg = fisher_add_frame(boat);
    if (pkg == NULL) {
        DEBUG("ERROR buffer full!\n");
        return ERR;
    }

    pkg->type = FISHER_FRAME_TYPE_DATA;
    pkg->originator = boat->addr;
    pkg->sender = boat->addr;
    pkg->recipient = recipient;
    memcpy(pkg->content, data, len);
    pkg->length = len;
    pkg->receiver = route->node_neighbour;
    pkg->hops = 0;
    printf("Sent!\n");
    return OK;
}


/**
 * gets called by the hardware and processed the received frame
 * @param boat
 * @param frame
 * @return status
 */
Status fisher_packet_read(struct fisher_boat* boat, struct fisher_frame *frame) {
    if (frame == NULL) {
        printf("Error! processing empty frame!");
        return ERR;
    }
    // drop packet if receiver does not match own address/broadcast
    if (frame->receiver != boat->addr && frame->receiver != BROADCAST) {
        //DEBUG("Dropping Packet because not receiver!\n");
        return OK;
    }
    // too many hops
    if (frame->hops > MAXIMUM_HOPS) {
        // drop packet
        DEBUG("!!!!!!!!!!!!!!!!!!!!!!!! TTL == %d, dropping pkg\n", MAXIMUM_HOPS);
        return OK;
    }

    if (frame->originator == boat->addr) {
        DEBUG("originator is me dropping pkg\n");
        return OK;
    }

    bool marked[256];
    memset(&marked, 0, sizeof(marked));
    switch (frame->type) {
        case FISHER_FRAME_TYPE_HELLO:
            printf("");
            DEBUG("Received HELLO form %d from %d Forwarding...\n", frame->originator, frame->sender);

            // adding to routing table
            struct fisher_route *old_route = fisher_route_get(boat, frame->originator);

            if (old_route == NULL) {
                fisher_route_insert(boat, frame->originator, frame->sender, frame->hops, boat->tick);
            } else if (old_route->hops > frame->hops) { // if the current route is the better one
                fisher_route_insert(boat, frame->originator, frame->sender, frame->hops, boat->tick);
            }

            // retransmit
            // iterate threw all
            int count = 0;
            for (int i = 0; i < sizeof(boat->routes) / sizeof(struct fisher_route); i++) {
                struct fisher_route *route = &(boat->routes[i]);
                Address receiver = route->node_address;
                Address next_hop = route->node_neighbour;

                if (!route->active) continue;
                count++;
                if (frame->sender == receiver) continue; // do not echo back to sender
                if (frame->sender == next_hop) continue; // do not echo
                if (frame->originator == next_hop) continue;
                if (marked[next_hop]) continue;
                marked[next_hop] = true;

                struct fisher_frame *frame_out = fisher_add_frame(boat);
                if (frame_out == NULL) {
                    printf("Error, cannot sent more frames, buffer full!\n");
                    break;
                }
                memcpy(frame_out, frame, sizeof(struct fisher_frame));

                frame_out->hops++;
                frame_out->sender = boat->addr;
                frame_out->receiver = next_hop;
            }
            break;
        case FISHER_FRAME_TYPE_DATA:
            DEBUG("Received Data...");
            if (frame->originator == boat->addr) {
                DEBUG("Received Frame!");
                fisher_frame_print(boat, frame);
                break;
            }
            if (frame->recipient == boat->addr) {
                // Handle packet
                DEBUG("Recieved Packet! with content \"%s\" and len %d", frame->content, frame->length);
                break;
            }
            DEBUG("Forwarding\n");
            struct fisher_route *route = fisher_route_get(boat, frame->recipient);
            if (route == NULL) {
                DEBUG("Sorry, host %d not found! Could not Forward...\n", frame->recipient);
                break;
            }
            struct fisher_frame *frame_out = fisher_add_frame(boat);
            if (frame_out == NULL) {
                DEBUG("Error, cannot sent more frames, buffer full!\n");
                break;
            }
            memcpy(frame_out, frame, sizeof(struct fisher_frame));

            frame_out->hops++;
            frame_out->sender = boat->addr;
            frame_out->receiver = route->node_neighbour;
            break;
    }
    return OK;
}
/**
 * get frame that should be sent by the harware
 * @param boat
 * @return frame
 */
struct fisher_frame * fisher_frame_get_to_be_sent(struct fisher_boat *boat) {
    if (boat->to_be_sent_count == 0) return NULL;
    boat->to_be_sent_count--;
    struct fisher_frame *ret = &boat->to_be_sent[boat->to_be_sent_read];
    boat->to_be_sent_read = (boat->to_be_sent_read + 1) % MAX_FRAME_BUFFER_SIZE;
    return ret;
}

/**
 * Adds a frame to the to be sended queue returns null if queue is full
 * @param boat
 * @return fisher_frame
 */
struct fisher_frame * fisher_add_frame(struct fisher_boat *boat) {
    if (boat->to_be_sent_count == MAX_FRAME_BUFFER_SIZE) return NULL;
    struct fisher_frame *ret = &(boat->to_be_sent[boat->to_be_sent_write]);
    boat->to_be_sent_write = (boat->to_be_sent_write + 1) % MAX_FRAME_BUFFER_SIZE;
    boat->to_be_sent_count++;
    memset(ret, 0, sizeof(struct fisher_frame)); // for security
    return ret;
}
/**
 * generates hello packet
 * @param boat
 * @return status
 */
Status fisher_frame_generate_hello(struct fisher_boat *boat) {
    struct fisher_frame *pkg = fisher_add_frame(boat);
    if (pkg == NULL) {
        DEBUG("ERROR buffer full!\n");
        return ERR;
    }

    pkg->type = FISHER_FRAME_TYPE_HELLO;
    pkg->originator = boat->addr;
    pkg->sender = boat->addr;
    pkg->recipient = BROADCAST;
    pkg->receiver = BROADCAST;
    pkg->hops = 1;

    pkg->seq = boat->hello_seq;

    boat->hello_seq++;
    return OK;
}
/**
 * debug a frame
 * @param boat
 * @param frame
 */
void fisher_frame_print(struct fisher_boat *boat,  struct fisher_frame *frame) {
    printf("\n");
    DEBUG("----Fisher packet   type:");
    switch (frame->type) {
        case (FISHER_FRAME_TYPE_HELLO):
            printf("HELLO\n");
            DEBUG(" seq:\t%d\thops: %d\n", frame->seq, frame->hops);
            DEBUG("%d\t%d\t -> \t%d --> %d\n", frame->originator, frame->sender, frame->receiver, frame->recipient);
            break;
        default:
            DEBUG("UNKNOWN");
    }
    DEBUG("\n----\n");
}
/**
 * print routing table
 * @param boat
 */
void fisher_routing_debug(struct fisher_boat *boat) {
    DEBUG("Routes:\n");
    int count = 0;
    for (int i = 0; i < sizeof(boat->routes)/sizeof(struct fisher_route); i++) {
        struct fisher_route *route = &(boat->routes[i]);
        if (!route->active) continue;
        count++;
        DEBUG("route over %d\t\t->\t%d (destination) (%d)\n", route->node_neighbour, route->node_address, route->hops);
    }
    DEBUG("Total:\t%d\n", count);
}