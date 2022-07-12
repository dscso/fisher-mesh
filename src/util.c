#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "fisher.h"
#include "util.h"
#define SIZE 40




struct fisher_route* dummyItem;


int hashCode(uint8_t key) {
    return key % SIZE;
}

struct fisher_route *search(uint8_t node_address, struct fisher_boat *boat ) {
    //get the hash
    int hashIndex = hashCode(node_address);

    //move in array until an empty
    while(boat->routing_table[hashIndex] != NULL) {

        if(boat->routing_table[hashIndex]->node_address == node_address)
        {
            return boat->routing_table[hashIndex];
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
    struct fisher_route *item = (struct fisher_route*) malloc(sizeof(struct fisher_route));
    item->node_neighbour = node_neighbour;
    item->node_address = node_adress;

    //get the hash
    int hashIndex = hashCode(node_adress);

    //move in array until an empty or deleted cell
    while(boat->routing_table[hashIndex] != NULL && boat->routing_table[hashIndex]->node_address != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    boat->routing_table[hashIndex] = item;
}

struct fisher_route* delete(struct fisher_route* item , struct fisher_boat *boat) {
    uint8_t key = item->node_address;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(boat->routing_table[hashIndex] != NULL) {

        if(boat->routing_table[hashIndex]->node_address == key) {
            struct fisher_route* temp = boat->routing_table[hashIndex];

            //assign a dummy item at deleted position
            boat->routing_table[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

Status fisher_route_init(struct fisher_boat *boat) {
    memset(boat->routes, 0, sizeof(boat->routes));
    return OK;
}
Status fisher_route_insert(struct fisher_boat *boat, Address destination, Address neighbour, int hops) {
    printf("[%d]\t\tinsterting route to %d over %d\n",boat->addr, destination, neighbour);
    boat->routes[destination].active = true;
    boat->routes[destination].node_address = destination;
    boat->routes[destination].node_neighbour = neighbour;
    boat->routes[destination].hops = hops;

    return OK;
}


struct fisher_route* fisher_route_get(struct fisher_boat *boat, Address destination) {
    if (!boat->routes[destination].active) {
        return NULL;
    }
    return &boat->routes[destination];
}