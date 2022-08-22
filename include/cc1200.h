//
// Created by Jurek on 24.05.22.
//

#include "register.h"

/**
 * RecvPKG - struct that contains the content of a received packet
 * @size: size of packet
 * @content: the content of the received message
 * @rssi: signal strenght
 * @crc: CRC checksum
 */
typedef struct {
    size_t size;
    char content[255];
    int rssi;
    int crc;
} RecvPKG;

/**
 * CC_Enum - gets returned from all cc functions
 * @CC_ERROR: an error has occured
 * @CC_OK: No error
 */
enum {
    CC_ERROR = 1,
    CC_OK = 0
};

void cc_reset();

void cc_init(REG_TYPE *RegSettings_, REG_TYPE *ExtRegSettings_);

int cc_change_status(int mode, int expect);

void cc_send_pkg(char * content, size_t lenght);

void cc_receive_pkg(RecvPKG* pkg);
void cc_print_pkg(RecvPKG *pkg);

void cc_shutdown();
