#define MAX_FRAME_BUFFER_SIZE 100


typedef enum Status {
    OK = 0,
    ERR = 1
} Status;

typedef enum fisher_frame_type {
    HELLO,

} fisherFrameType;
typedef struct Address {

} Address;


// what should be serialized/deserialized and send/recieved via cc1200
// gets build by the agorithm
typedef struct fisher_frame {
    int type;
    Address sender;
    Address recipand;
    int ttl;
    int rssi;
    int length;
    char * content;
} fisherFrame;

// a DATA package that should be routed
typedef struct fisher_package {
    Address sender;
    Address receiver;
    int ttl;
    int length;
    char * content;
} fisherPackage;

typedef struct fisher_boat {
    Address addr;
    // routing table
    // routing
    fisherFrame to_be_sent[MAX_FRAME_BUFFER_SIZE];

} fisherBoat;

// inits the fisherBoat struct, sets address, inits clean routing table
// #pre empty boat
// #post boat contains address
Status fisher_init(fisherBoat *boat, Address addr);

// gets executed evey loop, generate HELLO frames
// #pre boat in old state
// #post if algorithm decides to send hello messages,
Status fisher_tick(fisherBoat *boat);

// generates data frames
Status fisher_packet_generate(fisherBoat *boat /* TODO */);
Status fisher_packet_read(fisherBoat* boat /* TODO */);

// responds to hello frames
// routes frames
Status fisher_frame_process(fisherBoat *boat, fisherFrame * frames, int len /* TODO */);
Status fisher_frame_get_to_be_sent(fisherBoat *boat, fisherFrame * frames, int *len /* TODO */);
