//
// Created by david on 05.07.22.
//

#ifndef FISHER_H_UTIL_H
#define FISHER_H_UTIL_H

#endif //FISHER_H_UTIL_H
int hashCode(uint8_t key);
struct fisher_route *search(uint8_t node_address, struct fisher_boat *boat );
void insert(uint8_t node_adress , uint8_t node_neighbour,struct fisher_boat *boat );
struct fisher_route* delete(struct fisher_route* item , struct fisher_boat *boat);


