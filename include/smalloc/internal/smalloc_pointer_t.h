#ifndef SMALLOC_SMALLOC_POINTER_T_H
#define SMALLOC_SMALLOC_POINTER_T_H

#include "smalloc/smalloc_result_t.h"

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "smalloc/internal/smalloc_flags.h"

#ifdef __SMALLOC_INTERNAL_FUNCTIONS

        typedef enum
        {
                SMALLOC_TYPE_SINGLE    = 1, // Single allocation
                SMALLOC_TYPE_PTR_ARRAY = 2  // Array of pointers
        } smalloc_alloc_type_t;

        typedef struct
        {
                void*                ptr;
                smalloc_alloc_type_t type;
                size_t               size;
                size_t               elem_size;
                uint32_t             flags;
        } smalloc_pointer_t;

        /**
         * Create a smalloc pointer structure
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to create
         * @param size Size of the memory to allocate
         * @param flags Flags for the type of allocation
         * @return Result code indicating success or failure
         */
        smalloc_result_t
        _i_smalloc_ptr_create_single( smalloc_pointer_t** ptr, size_t size, uint32_t flags );

        /**
         * Create a smalloc pointer structure for an array
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to create
         * @param arr_size Size of the array to allocate
         * @param elem_size Size of the individual array elements
         * @param flags Flags for the type of allocation
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_ptr_create_ptr_array( smalloc_pointer_t** ptr,
                                                          size_t              arr_size,
                                                          size_t              elem_size,
                                                          uint32_t            flags );

        /**
         * Reallocate a smalloc single pointer structure
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to reallocate
         * @param size Size of the memory to allocate
         * @param flags Flags for the type of allocation
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_ptr_realloc_single( smalloc_pointer_t** ptr, size_t size );

        /**
         * Reallocate a smalloc pointer structure for an array
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to reallocate
         * @param new_arr_size Size of the new array to allocate
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_ptr_realloc_array( smalloc_pointer_t** ptr,
                                                       size_t              new_arr_size );

        /**
         * Create a smalloc pointer structure from an existing pointer
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to create
         * @param memory Pointer to the existing memory
         * @param size Size of the memory
         * @param flags Flags for the type of allocation
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_create_from_ptr( smalloc_pointer_t** ptr,
                                                     void*               memory,
                                                     size_t              size,
                                                     uint32_t            flags );

        /**
         * Destroy a smalloc pointer structure
         *
         * @param ptr [out] Pointer to the smalloc pointer structure to destroy
         * @return Result code indicating success or failure
         */
        smalloc_result_t _i_smalloc_ptr_free( smalloc_pointer_t** ptr );

#endif

#ifdef __cplusplus
}
#endif

#endif // SMALLOC_SMALLOC_POINTER_T_H
