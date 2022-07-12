#include <stdio.h>
#include <unistd.h>
#include "fisher.h"
#define     ever    ;;

struct connection {
    Address v;
    Address w;
};

#define AMOUNT_OF_NODES     3
Address nodes[AMOUNT_OF_NODES] = {1, 2, 3};//, 4, 5};
static struct connection connections[] = {
        {0, 1},
        {1, 0},
        {2, 1},
        {1, 2},
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
            struct fisher_frame *frame = fisher_frame_get_to_be_sent(&(boat[connections[i].v]));

            while (frame != NULL) {
                fisher_frame_print(&(boat[i]), frame);
                for (int j = 0; j < amount_of_connections; j++) {
                    if (boat[connections[j].v].addr != boat[i].addr) continue;
                    printf("----- forwarding packet %d -> %d\n", boat[connections[j].v].addr, boat[connections[j].w].addr);
                    fisher_packet_read(&(boat[connections[j].w]), frame);
                }
                fisher_packet_read(&(boat[i]), frame);

                frame = fisher_frame_get_to_be_sent(&(boat[i]));
            }
        }

        usleep(10000); // evey 10 m
    }
    return 0;
}
