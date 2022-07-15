#include <stdio.h>
#include <unistd.h>
#include "fisher.h"
#define     ever    ;;

struct connection {
    Address v;
    Address w;
};

#define AMOUNT_OF_NODES     5
Address nodes[AMOUNT_OF_NODES] = {1, 2, 3, 4, 5};
static struct connection connections[] = {
        {0, 1},
        {1, 0},

        {2, 1},
        {1, 2},

        {2,3},
        {3, 2},

        {4, 3},
        {3, 4},

       // {2, 3}
};

int main() {
    printf("Hello, World!\n");
    const int amount_of_nodes = sizeof(nodes) / sizeof(Address);
    const int amount_of_connections = sizeof(connections) / sizeof(struct connection);
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
        for (int i = 0; i < amount_of_nodes; i++) {
            struct fisher_boat *current_boat = &(boat[i]);

            struct fisher_frame *frame = fisher_frame_get_to_be_sent(current_boat);
            while (frame != NULL) {
                fisher_frame_print(&(boat[i]), frame);
                for (int j = 0; j < amount_of_connections; j++) {
                    if (boat[connections[j].w].addr == current_boat->addr) {
                        printf("----- forwarding packet %d -> ... -> %d -> %d\n",  frame->originator, frame->sender, boat[connections[j].v].addr);
                        fisher_packet_read(&(boat[connections[j].v]), frame);
                    }
                }

                frame = fisher_frame_get_to_be_sent(current_boat);
            }
        }

        usleep(10000); // evey 10 m
    }
    return 0;
}
