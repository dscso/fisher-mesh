#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "fisher.h"
#include "bintool.h"

#define     ever    ;;

struct connection {
    Address v;
    Address w;
};

#define AMOUNT_OF_NODES     9
Address nodes[AMOUNT_OF_NODES] = {1, 2, 3, 4, 5,6,7, 8, 9};
static struct connection connections[] = {
        {0, 1},
        {1, 0},

        {2, 1},
        {1, 2},

        {2, 3},
        {3, 2},

        {4, 3},
        {3, 4},

        {5,4},
        {4,5},

        {6,5},
        {5,6},

        {6,7},
        {7,6},


        {8,7},
        {7,8},

        {8,0},
        {0,8}
};

int main() {
    printf("Hello, World!\n");
    const int amount_of_nodes = sizeof(nodes) / sizeof(Address);
    const int amount_of_connections = sizeof(connections) / sizeof(struct connection);
    struct fisher_boat boat[amount_of_nodes];

    for (int i = 0; i < amount_of_nodes; i++) {
        fisher_init(&(boat[i]), nodes[i]);
    }
    int count = 0;
    for (ever) {
        // tick all boats
        for (int i = 0; i < amount_of_nodes; i++) {
            fisher_tick(&(boat[i]));
        }
        // connect all boats
        for (int i = 0; i < amount_of_nodes; i++) { // go threw all boats
            struct fisher_boat *current_boat = &(boat[i]);
            struct fisher_frame *frame = NULL;

            while ((frame = fisher_frame_get_to_be_sent(current_boat)) != NULL) {  // for each frame to be sent
                // TODO david Serialize
                //fisher_frame_print(&(boat[i]), frame);
                for (int j = 0; j < amount_of_connections; j++) { // for evey connection
                    if (boat[connections[j].v].addr != current_boat->addr) continue;
                    uint8_t pkg[300];
                    fisher_serialize(frame, &pkg);
                    struct fisher_frame frm;
                    fisher_deserialize(&frm, &pkg);
                    //printf("[%d] forwarding packet from %d -> ... -> %d -> %d recipant: %d\n",boat[connections[j].w].addr,  frame->originator, frame->sender, boat[connections[j].w].addr, frame->recipient);
                    //printf("\t--- %d %d %d %d\n", frame->originator, frame->sender, frame->receiver, frame->recipient);
                    // TODO david deserialize
                    fisher_packet_read(&(boat[connections[j].w]), &frm);
                }
            }
            count++;
        }
        if (count % 11 * 100 == 0) {
            printf("Sending...!\n");
            char s[] = "hi das ist ein test";
            fisher_packet_generate(&boat[0], boat[2].addr, s , strlen(s));
        }
        usleep(10000); // evey 10 m
        //printf("--------------------- TICK ----------------------\n");
    }
    return 0;
}
