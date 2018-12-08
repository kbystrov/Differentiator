#ifndef DIFFERENTIATOR_DATA_TYPES_H
#define DIFFERENTIATOR_DATA_TYPES_H

#include <stddef.h>
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

typedef double elem_t;

typedef size_t tree_size_t;

typedef bool bool_t;

typedef enum{
    NONE = 0,
    VALUE = 1,
    OPERATOR = 2,
    VARIABLE = 3,
} DATA_TYPE;

#endif //DIFFERENTIATOR_DATA_TYPES_H
