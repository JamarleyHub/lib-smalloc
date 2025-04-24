#ifndef SMALLOC_SMALLOC_FLAGS_H
#define SMALLOC_SMALLOC_FLAGS_H

/**
 * Flags to set for individual smalloc pointers
 */
#define SMALLOC_FLAG_AUTO                  ( 1ULL << 0 )
#define SMALLOC_FLAG_DYNAMIC               ( 1ULL << 1 )
#define SMALLOC_FLAG_PERSISTENT            ( 1ULL << 2 )
#define SMALLOC_FLAG_PTR_ARRAY             ( 1ULL << 3 )

#define SMALLOC_SET_FLAG( flags, flag )    ( ( flags ) |= ( flag ) )
#define SMALLOC_CLEAR_FLAG( flags, flag )  ( ( flags ) &= ~( flag ) )
#define SMALLOC_CLEAR_FLAGS( flags )  ( ( flags ) &= 0 )
#define SMALLOC_TOGGLE_FLAG( flags, flag ) ( ( flags ) ^= ( flag ) )
#define SMALLOC_IS_FLAG_SET( flags, flag ) ( ( ( flags ) & ( flag ) ) != 0 )

#endif // SMALLOC_SMALLOC_FLAGS_H
