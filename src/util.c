#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "fisher.h"
#include "util.h"
#define SIZE 40




struct RoutingItem* dummyItem;


int hashCode(uint8_t key) {
    return key % SIZE;
}

struct RoutingItem *search(uint8_t node_address, struct fisher_boat *boat ) {
    //get the hash
    int hashIndex = hashCode(node_address);

    //move in array until an empty
    while(boat->routing_tabele[hashIndex] != NULL) {

        if(boat->routing_tabele[hashIndex]->node_address == node_address)
        {
            return boat->routing_tabele[hashIndex];
        }
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insert(uint8_t node_adress , uint8_t node_neighbour,struct fisher_boat *boat ) {

    printf("inserting route from %d -> %d",node_adress);
    struct RoutingItem *item = (struct RoutingItem*) malloc(sizeof(struct RoutingItem));
    item->node_neighbour = node_neighbour;
    item->node_address = node_adress;

    //get the hash
    int hashIndex = hashCode(node_adress);

    //move in array until an empty or deleted cell
    while(boat->routing_tabele[hashIndex] != NULL && boat->routing_tabele[hashIndex]->node_address != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    boat->routing_tabele[hashIndex] = item;
}

struct RoutingItem* delete(struct RoutingItem* item ,struct fisher_boat *boat) {
    uint8_t key = item->node_address;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(boat->routing_tabele[hashIndex] != NULL) {

        if(boat->routing_tabele[hashIndex]->node_address == key) {
            struct RoutingItem* temp = boat->routing_tabele[hashIndex];

            //assign a dummy item at deleted position
            boat->routing_tabele[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}
