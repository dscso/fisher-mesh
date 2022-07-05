#include <stdint.h>

#define MAX_FRAME_BUFFER_SIZE   100
#define ADDRESS_SIZE            8
#define Address                 uint8_t
#define ROUTINGSIZE             30

typedef enum Status {
    OK = 0,
    ERR = 1
} Status;

enum fisher_frame_type {
    FISHER_FRAME_TYPE_HELLO,

};

struct RoutingItem {
    uint8_t node_address;
    uint8_t node_neighbour;
};



// what should be serialized/deserialized and send/recieved via cc1200
// gets build by the agorithm
struct fisher_frame {
    int type;
    /*
     * OGM2
     */
    // TTL
    int seq;
    Address originator;
    /*
     * DATA Packet
     */
    Address sender;
    Address recipient;
    int ttl;
    int rssi;
    int length;
    char * content;
};

// a DATA package that should be routed
struct fisher_package {
    Address sender;
    Address receiver;
    int ttl;
    int length;
    char * content;
};

struct fisher_boat {
    Address addr;
    // routing table
    struct RoutingItem* routing_tabele[ROUTINGSIZE];
    //struct fisher_route route[ADDRESS_SIZE];
    // routing

    struct fisher_frame to_be_sent[MAX_FRAME_BUFFER_SIZE];
    int to_be_sent_read;
    int to_be_sent_write;
    int to_be_sent_count;


    int tick;
    int hello_evey_tick;
    int hello_seq;
};

// inits the fisherBoat struct, sets address, inits clean routing table
// #pre empty boat
// #post boat contains address
Status fisher_init(struct fisher_boat *boat, Address addr);

// gets executed evey loop, generate HELLO frames
// #pre boat in old state
// #post if algorithm decides to send hello messages,
Status fisher_tick(struct fisher_boat *boat);

// generates data frames
Status fisher_packet_generate(struct fisher_boat *boat /* TODO */);
Status fisher_packet_read(struct fisher_boat* boat /* TODO */);

// responds to hello frames
// routes frames
// if hardware receives a packet, it will be passed to the algorithm with this function
Status fisher_frame_process(struct fisher_boat *boat, struct fisher_frame * frames, int len /* TODO */);
struct fisher_frame * fisher_frame_get_to_be_sent(struct fisher_boat *boat);


Status fisher_frame_generate_hello(struct fisher_boat *boat);
void fisher_frame_print(struct fisher_frame *frame);