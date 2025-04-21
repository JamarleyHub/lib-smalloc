#ifndef SMALLOC_SMALLOC_MEM_LIST_H
#define SMALLOC_SMALLOC_MEM_LIST_H

#include "smalloc/internal/smalloc_pointer_t.h"
#include "smalloc/internal/smalloc_result_t.h"

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __SMALLOC_INTERNAL_FUNCTIONS

  #define _I_SMALLOC_INITIAL_LIST_CAPACITY 8

        typedef struct
        {
                smalloc_pointer_t** items;
                size_t              capacity;
                size_t              size;
        } smalloc_list_t;

        /**
         * Initialize a memory list with specified initial capacity
         *
         * @param list The list to initialize
         * @param init_cap Initial capacity for the list
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_init( smalloc_list_t** list, size_t init_cap );

        /**
         * Clean up and free all resources used by the list
         * Note: This does NOT free the memory pointed to by list items
         *
         * @param list The list to destroy
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_destroy( smalloc_list_t** list );

        /**
         * Add a pointer to the list
         *
         * @param list The list to add to
         * @param pointer The pointer structure to add
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_add( smalloc_list_t* list, smalloc_pointer_t* pointer );

        /**
         * Find a pointer in the list by its memory address
         *
         * @param list The list to search
         * @param ptr The memory address to find
         * @param index If not NULL and pointer is found, receives the index
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_find( const smalloc_list_t* list,
                                                  smalloc_pointer_t*    ptr,
                                                  size_t*               index );

        /**
         * Remove a pointer from the list by its memory address
         *
         * @param list The list to remove from
         * @param ptr The memory address to remove
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_memlist_remove( smalloc_list_t* list, smalloc_pointer_t* ptr );

        /**
         * Check if the list is empty
         *
         * @param list The list to check
         * @return true if empty or NULL, false otherwise
         */
        bool             _i_smalloc_memlist_is_empty( const smalloc_list_t* list );

#endif

#ifdef __cplusplus
}
#endif

#endif // SMALLOC_SMALLOC_MEM_LIST_H
