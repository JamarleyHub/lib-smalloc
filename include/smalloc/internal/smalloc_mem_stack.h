#ifndef SMALLOC_SMALLOC_MEM_STACK_H
#define SMALLOC_SMALLOC_MEM_STACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __SMALLOC_INTERNAL_FUNCTIONS
  #include "smalloc/smalloc_result_t.h"
  #include "smalloc_pointer_t.h"

  #include <stdbool.h>
  #include <stddef.h>
  #include <stdlib.h>

  #define _I_SMALLOC_INITIAL_STACK_CAPACITY 8

        /**
         * Stack structure to track allocated pointers in LIFO order.
         * NOTE: Members should not be manually changed!
         */
        struct smalloc_stack_t
        {
                smalloc_pointer_t** items;    // Array of pointers to allocated memory
                size_t              capacity; // Current capacity of the items array
                size_t              size;     // Current number of items in the stack
        };

        /**
         * Initialize a new stack with default capacity
         *
         * @param stack [out] Pointer to the stack to initialize
         * @param init_cap Initial capacity of the stack (should be >= 8)
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_init( struct smalloc_stack_t** stack,
                                                   size_t                   init_cap );

        /**
         * Clean up and free all resources used by the stack
         * NOTE: This does NOT free the memory pointed to by stack items
         *
         * @param stack [out] The stack to destroy
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_destroy( struct smalloc_stack_t** stack );

        /**
         * Push a pointer onto the stack
         *
         * @param stack [out] The stack to push to
         * @param ptr [out] Pointer to push
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_push( struct smalloc_stack_t* stack,
                                                   smalloc_pointer_t*      ptr );

        /**
         * Pop a pointer from the stack
         *
         * @param stack [out] The stack to pop from
         * @param ptr [out] Pointer to store the popped pointer
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_pop( struct smalloc_stack_t* stack,
                                                  smalloc_pointer_t**     ptr );

        /**
         * Peek at the top pointer of the stack without removing it.
         * NOTE: The pointer that is written into ptr may later be freed and become invalid.
         * This means it should not be used after peeking.
         *
         * @param stack [out] The stack to peek at
         * @param ptr [out] Pointer to store the top pointer
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_peek( const struct smalloc_stack_t* stack,
                                                   smalloc_pointer_t**           ptr );

        /**
         * Find a pointer in the stack
         *
         * @param stack The stack to search
         * @param ptr The pointer to find
         * @param index [out] Index of the found pointer
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_find( const struct smalloc_stack_t* stack,
                                                   const void*                   ptr,
                                                   size_t*                       index );

        /**
         * Retrieve a pointer from the stack at a specific index
         *
         * @param stack The stack to retrieve from, the pointer will not be popped from the stack
         * @param ptr [out] Pointer to store the retrieved pointer
         * @param index Index of the pointer to retrieve
         * @return SMALLOC_OK on success, or an error code on failure
         */
        smalloc_result_t _i_smalloc_memstack_retrieve_index( const struct smalloc_stack_t* stack,
                                                             smalloc_pointer_t**           ptr,
                                                             const size_t                  index );

        /**
         * Check if the stack is empty
         *
         * @param stack The stack to check
         * @return true if empty, false otherwise
         */
        bool             _i_smalloc_memstack_is_empty( const struct smalloc_stack_t* stack );

#endif

#ifdef __cplusplus
}
#endif

#endif // SMALLOC_SMALLOC_MEM_STACK_H
