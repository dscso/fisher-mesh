//
// Created by Jurek on 13.05.22.
//
#include <SPIv1.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cc1200.h"
/**
 * resets cc1200 needs to be executed on each programm start
 */
void cc_reset() {
    cc1200_cmd(SRES);
}
/**
 * initializes the cc1200 and sets all necessary registers
 * @param reg_settings register settings
 * @param ext_reg_settings extended registers that should be set
 */
void cc_init(REG_TYPE* reg_settings, REG_TYPE* ext_reg_settings) {
    for (int i = 0; i < MAX_REG; i++) {
        cc1200_reg_write(reg_settings[i].adr, reg_settings[i].val);
    }
    // Extended Register tests
    printf("INFO: extended Register writes\n");
    for(int i = 0; i < MAX_EXT_REG; i++){
        cc1200_reg_write(ext_reg_settings[i].adr, ext_reg_settings[i].val);
    }
}
/**
 * changes the status and waits for the expected status (blocking)
 * @param mode desired mode
 * @param expect block till the expected mode is set
 * @return code, CC_ERROR if there occurred and error or CC_OK if no error has occured
 */
int cc_change_status(int mode, int expect) {
    cc1200_cmd(mode);
    int status;
    // wait till the expected code is reached
    while (expect != (status = get_status_cc1200())) {
        // break if there is an error and return
        if (status == TX_FIFO_ERROR || status == RX_FIFO_ERROR) {
            printf("Error");
            print_status(status);
            return CC_ERROR;
        }
        //printf(". %s", get_status_cc1200_str());
        cc1200_cmd(SNOP);
    }
    return CC_OK;
}
/**
 * send packet
 * @param content data to be sent
 * @param lenght lenght of data
 */
void cc_send_pkg(char * content, size_t lenght) {
    cc1200_reg_write(FIFO, (int) lenght);
    for (int i = 0; i < lenght > 0; i++) {
        cc1200_reg_write(FIFO, (int) content[i]);
    }
    cc_change_status(STX, IDLE);
}


/**
 * receive packet
 * @param pkg
 * non blocking if received nothing blocking if receiving something
 */
void cc_receive_pkg(RecvPKG *pkg) {
    int tmp;
    pkg->size = cc1200_reg_read(FIFO, &tmp);
    // wait till recv eveything
    while(cc1200_reg_read(NUM_RXBYTES, &tmp) < pkg->size + 2);

    for (int i = 0; i < pkg->size; i++) {
        pkg->content[i] = (char) cc1200_reg_read(FIFO, &tmp);
    }
    pkg->rssi = cc1200_reg_read(FIFO, &tmp);
    pkg->crc = cc1200_reg_read(FIFO, &tmp);

    //cc1200_cmd(SFRX);

    if (cc1200_reg_read(NUM_RXBYTES, &tmp) == 0) {
        cc_change_status(SRX, RX);
    } else {
        printf("Error, still %d RX bytes present!\n",tmp);
    }

}
/**
 * Debug a packet and print the packet human readable
 * @param pkg
 */
void cc_print_pkg(RecvPKG *pkg) {
    printf("\033[1;33m");
    printf("Recv package(RSSI:\t%d, CRC: \t%d, size:\t%zu):\t\"", pkg->rssi, pkg->crc, pkg->size);
    //for (int i = 0; i < pkg->size; printf("%c", pkg->content[i++]));
    printf("\"\n");
    printf("\033[0m");
}

/**
 * should be called before progamm exits
 */
void cc_shutdown() {
    printf("Closing...");
    spi_shutdown();
    printf("yalla bye\n");
    exit(1);
}