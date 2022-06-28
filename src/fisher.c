//
// Created by Jurek on 21.06.22.
//
#include "fisher.h"

Status fisher_init(struct fisher_boat *boat, Address addr){
    // init all with 0, create new routing table etc
}

// gets executed evey loop, generate HELLO frames
// #pre boat in old state
// #post if algorithm decides to send hello messages,
Status fisher_tick(struct fisher_boat *boat) {
    // TODO
    // compare if time has passed to send ogm2 msg
    // generate ogm2 msg
    // send
}

// generates data frames
Status fisher_packet_generate(struct fisher_boat *boat /* TODO */) {

}
Status fisher_packet_read(struct fisher_boat* boat /* TODO */) {
    // TODO
    // check if it is a ogm msg and forward
}

// responds to hello frames
// routes frames
// if harware recieves a packet, it will be passed to the algorithm with this function
Status fisher_frame_process(struct fisher_boat *boat, struct fisher_frame * frames, int len /* TODO */) {

}
Status fisher_frame_get_to_be_sent(struct fisher_boat *boat, struct fisher_frame * frames, int *len /* TODO */) {
    // TODO
    // return the frames to be sendet
}