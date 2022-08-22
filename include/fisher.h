#include <stdint.h>
#include <stdbool.h>

#define MAX_FRAME_BUFFER_SIZE   128
#define ADDRESS_SIZE            8
#define Address                 uint8_t

#define ROUTINGSIZE             30
#define BROADCAST               0xff
#define MAXIMUM_HOPS             100

/**
 * Status - status of the Algorithm if there has been an error
 */
typedef enum Status {
    OK = 0,
    ERR = 1,
    HOST_NOT_FOUND
} Status;

/**
 * fisher_frame_type - what kind of data packet we are dealing with
 * @FISHER_FRAME_TYPE_HELLO: Discovery message
 * @FISHER_FRAME_TYPE_DATA: Data frame
 */
enum fisher_frame_type {
    FISHER_FRAME_TYPE_HELLO,
    FISHER_FRAME_TYPE_DATA
};

/**
 * fisher_route - route struct that defines route properties
 * @active: if this is false, the route does not exist
 * @node_address: the destination host
 * @node_address: the address where a packed that should be sent to the host has to be sent to
 * @last_update: lost tick when route got updated/confirmed
 * @hops: how many hops does the route include
 */
struct fisher_route {
    bool active;
    uint8_t node_address;
    uint8_t node_neighbour;
    int last_update;
    int hops;
};


/**
 * fisher_frame - frame struct contains all frame information
 * @type: fisher_frame_type
 * @seq: sequence number (just for hello packets)
 * @originator: who sent the packet in the first place
 * @sender: who sent it/relayed it
 * @receiver: next receiver
 * @recipient: final destination
 * @hops: how many hops did the frame pass (incrementing)
 * @rssi: singal strenght
 * @lenght: packet lenght (just for data packets)
 * @content: data packet content (just for data packets)
 * gets build by the algorithm and gets later serialized
 */
struct fisher_frame {
    int type;
    /*
     * OGM2
     */
    int seq; // TTL
    Address originator;
    Address sender;
    Address receiver;
    Address recipient;
    int hops;
    int rssi;
    int length;
    char content[255];
};
/**
 * fisher_boat - the context of the algorithm needs to be passed to all fisher functions
 * @addr: address
 * @fisher_route: all routes the node knows
 * @to_be_sent: frames to be sent by the hardware
 * @to_be_sent_read: buffer read position pointer
 * @to_be_sent_write: buffer write position pointer
 * @to_be_sent_count: buffer size
 * @tick: algorithm time
 * @hello_evey_tick: send a discovery message all x ticks
 * @hello_seq: current sequence number of discovery packets
 * this struct stores all information the algorithm needs to know. packets that need to be processed, current time etc.
 * No external modification intended just with the fisher_ functions
 */
struct fisher_boat {
    Address addr;
    // routing table
    //struct fisher_route route[ADDRESS_SIZE];
    // routing
    struct fisher_route routes[256];

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
Status fisher_packet_generate(struct fisher_boat *boat, Address recipient, char *data, int len);
Status fisher_packet_read(struct fisher_boat* boat, struct fisher_frame *frame);

// responds to hello frames
// routes frames
// if hardware receives a packet, it will be passed to the algorithm with this function
struct fisher_frame * fisher_frame_get_to_be_sent(struct fisher_boat *boat);


Status fisher_frame_generate_hello(struct fisher_boat *boat);
void fisher_frame_print(struct fisher_boat *boat,  struct fisher_frame *frame);

struct fisher_frame * fisher_add_frame(struct fisher_boat *boat);


Status fisher_route_init(struct fisher_boat *boat);
Status fisher_route_insert(struct fisher_boat *boat, Address destination, Address neighbour, int hops, int tick);
struct fisher_route* fisher_route_get(struct fisher_boat *boat, Address destination);
void fisher_routing_debug(struct fisher_boat *boat);