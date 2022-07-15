#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "fisher.h"
#include "util.h"

Status fisher_route_init(struct fisher_boat *boat) {
    memset(boat->routes, 0, sizeof(boat->routes));
    return OK;
}
Status fisher_route_insert(struct fisher_boat *boat, Address destination, Address neighbour, int hops) {
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