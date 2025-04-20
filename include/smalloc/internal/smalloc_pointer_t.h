#ifndef SMALLOC_SMALLOC_POINTER_T_H
#define SMALLOC_SMALLOC_POINTER_T_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Flags to set for individual smalloc pointers
 */
#define SMALLOC_FLAG_AUTO          ( 1ULL << 0 )
#define SMALLOC_FLAG_PERSIST       ( 1ULL << 1 )

#define SET_FLAG( flags, flag )    ( ( flags ) |= ( flag ) )
#define CLEAR_FLAG( flags, flag )  ( ( flags ) &= ~( flag ) )
#define TOGGLE_FLAG( flags, flag ) ( ( flags ) ^= ( flag ) )
#define IS_FLAG_SET( flags, flag ) ( ( ( flags ) & ( flag ) ) != 0 )

// #ifdef __SMALLOC_INTERNAL_FUNCTIONS

typedef struct
{
        void*    ptr;
        uint32_t flags;
        size_t   size;
} smalloc_pointer_t;

// #endif

#endif // SMALLOC_SMALLOC_POINTER_T_H
