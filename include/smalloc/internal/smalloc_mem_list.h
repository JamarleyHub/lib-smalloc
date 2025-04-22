#ifndef SMALLOC_SMALLOC_MEM_LIST_H
#define SMALLOC_SMALLOC_MEM_LIST_H

#include "smalloc/internal/smalloc_pointer_t.h"
#include "smalloc/smalloc_result_t.h"

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __SMALLOC_INTERNAL_FUNCTIONS

  #define _I_SMALLOC_INITIAL_LIST_CAPACITY 8

        struct smalloc_list_t
        {
                smalloc_pointer_t** items;
                size_t              capacity;
                size_t              size;
        };

        /**
         * Initialize a memory list with specified initial capacity
         *
         * @param list The list to initialize
         * @param init_cap Initial capacity for the list
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_init( struct smalloc_list_t** list, size_t init_cap );

        /**
         * Clean up and free all resources used by the list
         * Note: This does NOT free the memory pointed to by list items
         *
         * @param list The list to destroy
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_destroy( struct smalloc_list_t** list );

        /**
         * Add a pointer to the list
         *
         * @param list The list to add to
         * @param pointer The pointer structure to add
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_add( struct smalloc_list_t* list,
                                                 smalloc_pointer_t*     pointer );

        /**
         * Find a pointer in the list by its memory address
         *
         * @param list The list to search
         * @param ptr The memory address to find
         * @param index If not NULL and pointer is found, receives the index
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_find( const struct smalloc_list_t* list,
                                                  const smalloc_pointer_t*     ptr,
                                                  size_t*                      index );

        /**
         * Retrieve a pointer from the list by its index
         *
         * @param list The list to search
         * @param ptr If pointer is found, receives the pointer
         * @param index The index of the pointer to retrieve
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_retrieve_index( const struct smalloc_list_t* list,
                                                            smalloc_pointer_t**          ptr,
                                                            size_t                       index );

        /**
         * Remove a pointer from the list by its memory address
         *
         * @param list The list to remove from
         * @param ptr The memory address to remove
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_remove( struct smalloc_list_t* list,
                                                    smalloc_pointer_t*     ptr );

        /**
         * Free all memory allocated within the given list
         *
         * @param list The list to free
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_free( struct smalloc_list_t* list );

        /**
         * Check if the list is empty
         *
         * @param list The list to check
         * @return true if empty or NULL, false otherwise
         */
        bool             _i_smalloc_memlist_is_empty( const struct smalloc_list_t* list );

#endif

#ifdef __cplusplus
}
#endif

#endif // SMALLOC_SMALLOC_MEM_LIST_H
