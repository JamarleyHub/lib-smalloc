#define __SMALLOC_INTERNAL_FUNCTIONS

#include "smalloc/internal/smalloc_pointer_t.h"

smalloc_result_t
_i_smalloc_ptr_create_single( smalloc_pointer_t** ptr, const size_t size, const uint32_t flags ) {
        if ( 0 == size ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                return SMALLOC_ERR_WRONG_PTR_CREATE_CALL;
        }

        *ptr = malloc( sizeof( smalloc_pointer_t ) );
        if ( NULL == *ptr ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *ptr )->ptr = malloc( size );
        if ( NULL == ( *ptr )->ptr ) {
                free( *ptr );
                *ptr = NULL;
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }
        ( *ptr )->type      = SMALLOC_TYPE_SINGLE;
        ( *ptr )->size      = size;
        ( *ptr )->elem_size = 0;
        ( *ptr )->flags     = flags;

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_ptr_create_ptr_array( smalloc_pointer_t** ptr,
                                                  const size_t        arr_size,
                                                  const size_t        elem_size,
                                                  const uint32_t      flags ) {
        if ( 0 == arr_size ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( !SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                return SMALLOC_ERR_WRONG_PTR_CREATE_CALL;
        }

        *ptr = malloc( sizeof( smalloc_pointer_t ) );
        if ( NULL == *ptr ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *ptr )->ptr = malloc( arr_size * sizeof( void* ) );
        if ( NULL == ( *ptr )->ptr ) {
                free( *ptr );
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }
        void** ptr_array = (void**) ( *ptr )->ptr;
        for ( size_t i = 0; i < arr_size; i++ ) {
                ptr_array[i] = malloc( elem_size );
                if ( NULL == ptr_array[i] ) {
                        for ( size_t j = 0; j < i; j++ ) {
                                free( ptr_array[j] );
                        }
                        free( ( *ptr )->ptr );
                        free( *ptr );
                        *ptr = NULL;
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }
        }

        ( *ptr )->type      = SMALLOC_TYPE_PTR_ARRAY;
        ( *ptr )->size      = arr_size;
        ( *ptr )->elem_size = elem_size;
        ( *ptr )->flags     = flags;

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_ptr_realloc_single( smalloc_pointer_t** ptr, size_t size ) {
        if ( NULL == *ptr || 0 == size ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }
        if ( SMALLOC_TYPE_SINGLE != ( *ptr )->type ) {
                return SMALLOC_ERR_INVALID_PTR_TYPE;
        }
        if ( size == ( *ptr )->size ) {
                return SMALLOC_OK;
        }

        void* new_ptr = realloc( ( *ptr )->ptr, size );
        if ( NULL == new_ptr ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *ptr )->ptr  = new_ptr;
        ( *ptr )->size = size;

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_ptr_realloc_array( smalloc_pointer_t** ptr, size_t new_arr_size ) {
        if ( NULL == *ptr || 0 == new_arr_size ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }
        if ( SMALLOC_TYPE_PTR_ARRAY != ( *ptr )->type ) {
                return SMALLOC_ERR_INVALID_PTR_TYPE;
        }
        if ( new_arr_size == ( *ptr )->size ) {
                return SMALLOC_OK;
        }

        // Handle the case where the new size is smaller than the current size
        // This means we need to preserve the old element pointers and free them if the realloc
        // succeeds
        if ( new_arr_size < ( *ptr )->size ) {
                void** tmp_elements = malloc( ( ( *ptr )->size - new_arr_size ) * sizeof( void* ) );
                if ( NULL == tmp_elements ) {
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }
                void** ptr_array = (void**) ( *ptr )->ptr;
                for ( size_t i = new_arr_size; i < ( *ptr )->size; i++ ) {
                        tmp_elements[i - new_arr_size] = ptr_array[i];
                }
                void** new_ptr = realloc( ( *ptr )->ptr, new_arr_size * sizeof( void* ) );
                if ( NULL == new_ptr ) {
                        free( tmp_elements );
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }
                for ( size_t i = new_arr_size; i < ( *ptr )->size; i++ ) {
                        free( tmp_elements[i - new_arr_size] );
                }
                free( tmp_elements );
                ( *ptr )->ptr = new_ptr;
                return SMALLOC_OK;
        }

        // Handle the case where the new size is larger than the current size
        size_t original_size = ( *ptr )->size;
        void** new_ptr       = realloc( ( *ptr )->ptr, new_arr_size * sizeof( void* ) );
        if ( NULL == new_ptr ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *ptr )->ptr    = new_ptr;
        void** ptr_array = (void**) ( ( *ptr )->ptr );

        // Try to allocate each new element
        for ( size_t i = original_size; i < new_arr_size; i++ ) {
                ptr_array[i] = malloc( ( *ptr )->elem_size );
                if ( NULL == ptr_array[i] ) {
                        // Failed to allocate - clean up any new elements we did allocate
                        for ( size_t j = original_size; j < i; j++ ) {
                                free( ptr_array[j] );
                        }
                        // Shrink the array back to original size
                        void** shrunk_ptr =
                            realloc( ( *ptr )->ptr, original_size * sizeof( void* ) );
                        if ( shrunk_ptr == NULL ) {
                                // Failed to shrink - but original data is still intact
                                // Just keep using the larger array but only utilize the original
                                // portion
                                ( *ptr )->size = original_size;
                                return SMALLOC_ERR_SHRINK_FAILED;
                        }
                        ( *ptr )->ptr  = shrunk_ptr;
                        ( *ptr )->size = original_size;
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }
        }

        ( *ptr )->size = new_arr_size;
        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_ptr_free( smalloc_pointer_t** ptr ) {
        if ( NULL == *ptr ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }
        if ( SMALLOC_TYPE_PTR_ARRAY == ( *ptr )->type || SMALLOC_TYPE_SINGLE == ( *ptr )->type ) {
                return SMALLOC_ERR_INVALID_PTR_TYPE;
        }
        if ( 0 == ( *ptr )->size ) {
                return SMALLOC_ERR_NO_PTR_FOUND;
        }

        // Deallocation for single alloc pointers
        if ( SMALLOC_TYPE_SINGLE == ( *ptr )->type ) {
                ( *ptr )->type      = 0;
                ( *ptr )->size      = 0;
                ( *ptr )->elem_size = 0;
                ( *ptr )->flags     = 0;
                if ( NULL == ( *ptr )->ptr ) {
                        free( *ptr );
                        *ptr = NULL;
                        return SMALLOC_ERR_NO_PTR_FOUND;
                }
                free( ( *ptr )->ptr );
                free( *ptr );
                *ptr = NULL;
                return SMALLOC_OK;
        }

        // Deallocation for array of pointers
        if ( NULL == ( *ptr )->ptr || 0 == ( *ptr )->elem_size ) {
                ( *ptr )->type      = 0;
                ( *ptr )->size      = 0;
                ( *ptr )->elem_size = 0;
                ( *ptr )->flags     = 0;
                free( *ptr );
                *ptr = NULL;
                return SMALLOC_ERR_NO_PTR_FOUND;
        }

        void** ptr_array = (void**) ( ( *ptr )->ptr );
        for ( size_t i = 0; i < ( *ptr )->size; i++ ) {
                free( ptr_array[i] );
        }
        free( ( *ptr )->ptr );
        ( *ptr )->ptr       = NULL;
        ( *ptr )->type      = 0;
        ( *ptr )->size      = 0;
        ( *ptr )->elem_size = 0;
        ( *ptr )->flags     = 0;
        free( *ptr );
        *ptr = NULL;
        return SMALLOC_OK;
}
