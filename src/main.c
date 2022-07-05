#include <stdio.h>
#include <unistd.h>
#include "fisher.h"
#include "util.h"
#define     ever    ;;

int main() {
    printf("Hello, World!\n");
    struct fisher_boat boat;
    fisher_init(&boat, 123);
    printf("for ever\n");
    printf("start inserting");
    for (int i = 0; i < ROUTINGSIZE; ++i) {
        insert(i,i+1,&boat);
    }
    for (int i = 0; i < ROUTINGSIZE/2; ++i) {
        delete(search(i,&boat),&boat);
    }
    insert(3,14,&boat);

    printf("Ende");


//    for (ever) {
//        fisher_tick(&boat);
//        struct fisher_frame *frame = fisher_frame_get_to_be_sent(&boat);
//        if (frame != NULL) {
//            fisher_frame_print(frame);
//        }
//        usleep(10000); // evey 10 m
//    }


    return 0;
}
