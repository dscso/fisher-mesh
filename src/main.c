#include <stdio.h>
#include <unistd.h>
#include "fisher.h"
#include "util.h"
#define     ever    ;;

int main() {
    printf("Hello, World!\n");
    struct fisher_boat boat;
    struct fisher_boat boat2;
    fisher_init(&boat, 123);
    fisher_init(&boat2, 111);


    for (ever) {
        fisher_tick(&boat);
        fisher_tick(&boat2);

        struct fisher_frame *frame2 = fisher_frame_get_to_be_sent(&boat2);
        int count = 0;
        while (frame2 != NULL) {
            printf("Sending form boat1 to boat2\n");
            fisher_packet_read(&boat, frame2);
            frame2 = fisher_frame_get_to_be_sent(&boat2);
        }



        struct fisher_frame *frame = fisher_frame_get_to_be_sent(&boat);
        while (frame != NULL) {
            printf("-012-3123123123123");
            fisher_frame_print(frame);
            count++;
            frame = fisher_frame_get_to_be_sent(&boat);
        }
        if (count != 0) printf("Sended %d\n", count);
        usleep(10000); // evey 10 m
    }
    return 0;
}
