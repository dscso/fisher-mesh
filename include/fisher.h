#include <stdint.h>

#define MAX_FRAME_BUFFER_SIZE   100
#define ADDRESS_SIZE            8


typedef enum Status {
    OK = 0,
    ERR = 1
} Status;

typedef enum fisher_frame_type {
    OGM2,

} fisherFrameType;
typedef struct Address {
    uint8_t Addr;
} Address;


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
    //struct fisher_route route[ADDRESS_SIZE];
    // routing
    struct fisher_frame to_be_sent[MAX_FRAME_BUFFER_SIZE];

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
Status fisher_frame_get_to_be_sent(struct fisher_boat *boat, struct fisher_frame * frames, int *len /* TODO */);
