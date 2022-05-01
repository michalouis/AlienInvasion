#include "ADTSet.h"
#include <stdio.h>
#include <stdlib.h>

Pointer set_find_eq_or_greater(Set set, Pointer value) {
    if (set_find(set, value) != NULL) { // if set contains value,
        return value;                   // return value
    } else {
        set_insert(set, value); // insert value in set
        SetNode node = set_find_node(set, value); // find value's node

        if (node != set_last(set)) {    // if there's a greater value
            value = set_node_value(set, set_next(set, node));   // recover greater value
            set_remove(set, set_node_value(set, node)); // remove value from set
            return value;   // return greater value
        } else {    // if there's no greater value
            set_remove(set, value); // remove value from set
            return NULL;    // return NULL
        }
    }
}

Pointer set_find_eq_or_smaller(Set set, Pointer value) {
    if (set_find(set, value) != NULL) { // if set contains value,
        return value;                   // return value
    } else {
        set_insert(set, value); // insert value in set
        SetNode node = set_find_node(set, value);   // find value's node

        if (node != set_first(set)) {   // if there's a greater value
            value = set_node_value(set, set_previous(set, node));   // recover greater value
            set_remove(set, set_node_value(set, node)); // remove value from set
            return value;   // return greater value
        } else {    // if there's no greater value
            set_remove(set, value); // remove value from set
            return NULL;    // return NULL
        }
    }
}