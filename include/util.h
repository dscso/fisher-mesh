//
// Created by david on 05.07.22.
//

#ifndef FISHER_H_UTIL_H
#define FISHER_H_UTIL_H

#endif //FISHER_H_UTIL_H
Status fisher_route_init(struct fisher_boat *boat);
Status fisher_route_insert(struct fisher_boat *boat, Address destination, Address neighbour, int hops);
struct fisher_route* fisher_route_get(struct fisher_boat *boat, Address destination);