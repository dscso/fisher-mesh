#include <stdio.h>
#include <signal.h>
#include <SPIv1.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "cc1200.h"
#include "fisher.h"
#include "bintool.h"


#define ADDRESS 42
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"


struct connection {
    Address v;
    Address w;
};

#define AMOUNT_OF_NODES     3
Address nodes[AMOUNT_OF_NODES] = {1, 2, 3};
static struct connection connections[] = {
        {0, 1},
        {1, 0},

        {2, 1},
        {1, 2},
};



int main(int argc, char *argv[]) {
    if (spi_init()) {
        printf("ERROR: Initialization failed\n");
        return -1;
    }
    signal(SIGINT, cc_shutdown);
    cc_reset();
    cc_init(RegSettingsSending, ExtRegSettingsSending);


    /*
     * FISHER
     */
    struct fisher_boat boat;
    Address a = ADDRESS;
    if (argc >= 2) {
        a = atoi(argv[1]);
        printf("SETTING ADDERSS TO: %d\n", a);
    }
    fisher_init(&boat, a);


    const int amount_of_nodes = sizeof(nodes) / sizeof(Address);
    const int amount_of_connections = sizeof(connections) / sizeof(struct connection);
    struct fisher_boat boats[amount_of_nodes];

    for (int i = 0; i < amount_of_nodes; i++) {
        fisher_init(&(boats[i]), nodes[i]);
    }

    for (int counter = 0; 1; counter++) {
        fisher_tick(&boat);
        if (boat.tick % 10 == 0) {
            char s[200];
            sprintf(s, "Hello world, %d", boat.tick);
            fisher_packet_generate(&boats[amount_of_nodes - 1], 41, s, strlen(s));
        }
        // tick all other boats
        for (int i = 0; i < amount_of_nodes; i++) {
            fisher_tick(&(boats[i]));
        }

        // connect all boats
        for (int i = 0; i < amount_of_nodes; i++) { // go threw all boats
            struct fisher_boat *current_boat = &(boats[i]);
            struct fisher_frame *frame = NULL;

            while ((frame = fisher_frame_get_to_be_sent(current_boat)) != NULL) {  // for each frame to be sent
                for (int j = 0; j < amount_of_connections; j++) { // for evey connection
                    if (boats[connections[j].v].addr != current_boat->addr) continue;
                    fisher_packet_read(&(boats[connections[j].w]), frame);
                }
                if (i == 0) {
                    fisher_packet_read(&boat, frame);
                }
            }
        }


        struct fisher_frame *frame;

        // THE REAL BOAT


        while ((frame = fisher_frame_get_to_be_sent(&boat)) != NULL) {
            usleep(10000);
            uint8_t buf[255];
            int pkg_len = fisher_serialize(frame, buf);
            cc_send_pkg(&buf, pkg_len);
            printf("%sSended... %s\n", YELLOW, RESET);

            fisher_packet_read(&boats[0], frame);
        }

        usleep(10000);

        if (cc_change_status(SRX, RX) == CC_ERROR) break;
        RecvPKG pkg;
        for (int i = 0; i < 5000; i++) {
            int tmp;
            if (cc1200_reg_read(NUM_RXBYTES, &tmp) != 0) {
                cc_receive_pkg(&pkg);
                cc_print_pkg(&pkg);
                struct fisher_frame frame_to_send;
                fisher_deserialize(&frame_to_send, (uint8_t *) &pkg.content);
                fisher_packet_read(&boat, &frame_to_send);
            }
        }
        if (get_status_cc1200() != IDLE)
            if (cc_change_status(SIDLE, IDLE) == CC_ERROR) break;
        if (boat.tick % 10 == 0) {
            fisher_routing_debug(&boat);
        }
        //usleep(10000);
    }

    return 1;
}