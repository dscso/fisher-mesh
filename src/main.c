#include <stdio.h>
#include <unistd.h>
#include "fisher.h"
#define     ever    ;;

struct connection {
    Address v;
    Address w;
};

#define AMOUNT_OF_NODES     5
Address nodes[AMOUNT_OF_NODES] = {0, 1, 2, 3, 4};
static struct connection connections[] = {
        {0, 1},
        {1, 2},
        {2, 3}
};

int main() {
    printf("Hello, World!\n");
    const amount_of_nodes = sizeof(nodes) / sizeof(Address);
    const amount_of_connections = sizeof(connections) / sizeof(struct connection);
    struct fisher_boat boat[amount_of_nodes];

    for (int i = 0; i < amount_of_nodes; i++) {
        fisher_init(&(boat[i]), nodes[i]);
    }



    for (ever) {
        // tick all boats
        for (int i = 0; i < amount_of_nodes; i++) {
            fisher_tick(&(boat[i]));
        }
        // connect all boats
        for (int i = 0; i < amount_of_connections; i++) {
            struct fisher_frame *frame = fisher_frame_get_to_be_sent(&(boat[connections[i].v]));
            while (frame != NULL) {
                //fisher_frame_print(frame);
                fisher_packet_read(&(boat[connections[i].w]), frame);
                frame = fisher_frame_get_to_be_sent(&(boat[connections[i].v]));
            }
        }
        // other way around
        for (int i = 0; i < amount_of_connections; i++) {
            struct fisher_frame *frame = fisher_frame_get_to_be_sent(&(boat[connections[i].w]));
            while (frame != NULL) {
                //fisher_frame_print(frame);
                fisher_packet_read(&(boat[connections[i].v]), frame);
                frame = fisher_frame_get_to_be_sent(&(boat[connections[i].w]));
            }
        }

        usleep(10000); // evey 10 m
    }
    return 0;
}
