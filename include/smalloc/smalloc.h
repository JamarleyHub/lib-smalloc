#ifndef SMALLOC_LIBRARY_H
#define SMALLOC_LIBRARY_H

#define __DEBUG

#include "smalloc_result_t.h"
#include "smalloc/internal/smalloc_flags.h"

#include <stdint.h>
#include <stdlib.h>

#ifdef __DEBUG
  #include <stdio.h>
#endif

#include "smalloc/internal/smalloc_guards.h"

#ifdef __cplusplus
extern "C"
{
#endif

        typedef struct smalloc_stack_t smalloc_stack_t;
        typedef struct smalloc_list_t  smalloc_list_t;

        typedef struct
        {
                size_t current_allocations_stack;
                size_t current_allocations_list;
                size_t total_allocations_freed;
                size_t total_allocations;
        } smalloc_stats_t;

        /**
         * The smalloc context type. This is used to manage memory allocations.
         * DO NOT MANUALLY EDIT ALLOCATIONS IN THIS CONTEXT!
         */
        typedef struct
        {
                smalloc_stack_t* alloc_stack;
                smalloc_list_t*  alloc_list;
                smalloc_stats_t  stats;
                smalloc_result_t last_operation_result;
        } smalloc_ctx_t;

        /**
         * Initializes the smalloc context type.
         *
         * @param ctx Pointer to an empty smalloc context
         * @param size Size of the context
         * @return SMALLOC_OK on success, error on failure
         */
        __SMALLOC_API smalloc_result_t smalloc_init_ctx( smalloc_ctx_t** ctx );

        /**
         * Destroys the smalloc context type. This will free all memory.
         *
         * @param ctx Pointer to a smalloc context
         */
        __SMALLOC_API void             smalloc_free_destroy_ctx( smalloc_ctx_t** ctx );

        /**
         * Destroys the smalloc context type. This will <b>NOT</b> free any memory.
         *
         * @param ctx Pointer to a smalloc context
         */
        __SMALLOC_API void             smalloc_destroy_ctx( smalloc_ctx_t** ctx );

        /**
         * Allocates memory of the given size.
         *
         * @param size Size of the memory to allocate
         * @param ctx Pointer to a smalloc context
         * @param flags Flags for the type of allocation, such as flags for allocating arrays or
         * allocating persistent memory to manually clean up
         * @return Pointer to the allocated memory
         */
        __SMALLOC_ALLOCATOR_API void*
        smalloc_alloc( size_t size, smalloc_ctx_t* ctx, uint32_t flags );

        /**
         * Allocates an array of pointers of the given size, with the elements being elem_size big.
         *
         * @param size Size of the memory to allocate
         * @param elem_size Size of a single element in the array
         * @param ctx Pointer to a smalloc context
         * @param flags Flags for the type of allocation, such as flags for allocating arrays or
         * allocating persistent memory to manually clean up
         * @return Pointer to the allocated memory
         */
        __SMALLOC_ALLOCATOR_API void*
        smalloc_alloc_arr( size_t size, size_t elem_size, smalloc_ctx_t* ctx, uint32_t flags );

        /**
         * Reallocates memory of the given size.
         * NOTE: Reallocating a pointer is only allowed, if the pointer was marked as
         * SMALLOC_FLAG_PERSIST.
         * If the pointer was not marked as SMALLOC_FLAG_PERSIST, the function will return NULL.
         *
         * @param ctx Pointer to a smalloc context
         * @param size Size of the memory to allocate
         * @return Pointer to the reallocated memory
         */
        __SMALLOC_ALLOCATOR_API void*
        smalloc_realloc( smalloc_ctx_t* ctx, void* ptr, size_t new_size );

        /**
         * Reallocates memory of the given size. This will reallocate memory within the smalloc
         * context.
         *
         * @param ctx Pointer to a smalloc context
         * @param size Size of the memory to allocate
         * @return Pointer to the reallocated memory
         */
        __SMALLOC_ALLOCATOR_API void*
                           smalloc_realloc_arr( smalloc_ctx_t* ctx, void* ptr, size_t new_size );

        /**
         * Adds a manually allocated pointer to the smalloc context to be managed by smalloc.
         *
         * @param ctx Pointer to a smalloc context
         * @param size Size of the memory to allocate
         * @param flags Flags for the type of allocation
         * @return Pointer to the allocated memory
         */
        __SMALLOC_API void smalloc_add_ptr_to_ctx( smalloc_ctx_t* ctx, void* ptr, uint32_t flags );

        /**
         * Frees all memory allocated within the given smalloc context.
         *
         * @param ctx The smalloc context to free
         */
        __SMALLOC_API void smalloc_free_all( smalloc_ctx_t* ctx );

        /**
         * Frees all memory allocated within the given smalloc context stack.
         *
         * @param ctx The smalloc context to free
         */
        __SMALLOC_API void smalloc_free_stack( smalloc_ctx_t* ctx );

        /**
         * Frees all memory allocated within the given smalloc context list.
         *
         * @param ctx The smalloc context to free
         */
        __SMALLOC_API void smalloc_free_list( smalloc_ctx_t* ctx );

        /**
         * Frees a specific pointer within the smalloc context.
         *
         * @param ctx The smalloc context to free
         */
        __SMALLOC_API void smalloc_free( smalloc_ctx_t* ctx, void* ptr );

        /**
         * Free only allocations with a specific flag type
         *
         * @param ctx The smalloc context containing the allocations
         * @param type_flags The type flags to free
         */
        //__SMALLOC_API void  smalloc_free_by_type( smalloc_ctx_t* ctx, uint32_t type_flags
        //);

#ifdef __DEBUG
                /**
                 * Dumps allocation information for debugging purposes
                 *
                 * @param ctx The smalloc context containing the allocations
                 * @param output The output file pointer to dump to (Closed in the function)
                 */
                //__SMALLOC_INTERNAL void smalloc_debug_dump( smalloc_ctx_t* ctx, FILE* output );
#endif

#ifdef __cplusplus
}
#endif

#endif // SMALLOC_LIBRARY_H
