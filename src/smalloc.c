#define __SMALLOC_INTERNAL_FUNCTIONS

#include "smalloc/internal/smalloc_internal.h"

#include "smalloc/smalloc.h"

__SMALLOC_API smalloc_result_t smalloc_init_ctx( smalloc_ctx_t** ctx ) {
        *ctx = (smalloc_ctx_t*) malloc( sizeof( smalloc_ctx_t ) );
        if ( *ctx == NULL ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *ctx )->alloc_stack = NULL;
        smalloc_result_t res  = _i_smalloc_memstack_init( &( ( *ctx )->alloc_stack ),
                                                         _I_SMALLOC_INITIAL_STACK_CAPACITY );
        if ( res != SMALLOC_OK ) {
                free( ctx );
                return res;
        }

        ( *ctx )->alloc_list = NULL;
        res =
            _i_smalloc_memlist_init( &( ( *ctx )->alloc_list ), _I_SMALLOC_INITIAL_LIST_CAPACITY );
        if ( res != SMALLOC_OK ) {
                _i_smalloc_memstack_destroy( &( ( *ctx )->alloc_stack ) );
                free( ctx );
                return res;
        }

        ( *ctx )->stats                 = (smalloc_stats_t) { 0 };
        ( *ctx )->last_operation_result = SMALLOC_OK;
        return SMALLOC_OK;
}

__SMALLOC_API void smalloc_free_destroy_ctx( smalloc_ctx_t** ctx ) {
        if ( NULL == ctx || NULL == *ctx ) {
                return;
        }
        smalloc_free_all( *ctx );
        _i_smalloc_memstack_destroy( &( ( *ctx )->alloc_stack ) );
        _i_smalloc_memlist_destroy( &( ( *ctx )->alloc_list ) );
        ( *ctx )->alloc_stack           = NULL;
        ( *ctx )->alloc_list            = NULL;
        ( *ctx )->stats                 = (smalloc_stats_t) { 0 };
        ( *ctx )->last_operation_result = SMALLOC_OK;
        free( *ctx );
        *ctx = NULL;
}

__SMALLOC_API void smalloc_destroy_ctx( smalloc_ctx_t** ctx ) {
        if ( NULL == ctx || NULL == *ctx ) {
                return;
        }
        _i_smalloc_memstack_destroy( &( ( *ctx )->alloc_stack ) );
        _i_smalloc_memlist_destroy( &( ( *ctx )->alloc_list ) );
        ( *ctx )->alloc_stack           = NULL;
        ( *ctx )->alloc_list            = NULL;
        ( *ctx )->stats                 = (smalloc_stats_t) { 0 };
        ( *ctx )->last_operation_result = SMALLOC_OK;
        free( *ctx );
        *ctx = NULL;
}

__SMALLOC_API void* smalloc_alloc( const size_t size, smalloc_ctx_t* ctx, uint32_t flags ) {
        if ( NULL == ctx || 0 == size ) {
                return NULL;
        }

        if ( SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                SMALLOC_CLEAR_FLAG( flags, SMALLOC_FLAG_PTR_ARRAY );
        }

        smalloc_pointer_t* ptr = NULL;
        smalloc_result_t   res = _i_smalloc_ptr_create_single( &ptr, size, flags );
        if ( SMALLOC_OK != res ) {
                return NULL;
        }
        ctx->stats.total_allocations++;

        if ( SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PERSIST ) ) {
                _i_smalloc_memlist_add( ctx->alloc_list, ptr );
                ctx->stats.current_allocations_list++;
                return ptr->ptr;
        }

        _i_smalloc_memstack_push( ctx->alloc_stack, ptr );
        ctx->stats.current_allocations_stack++;
        return ptr->ptr;
}

__SMALLOC_API void*
smalloc_alloc_arr( const size_t size, size_t elem_size, smalloc_ctx_t* ctx, uint32_t flags ) {
        if ( NULL == ctx || 0 == size || 0 == elem_size ) {
                return NULL;
        }

        if ( !SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                SMALLOC_SET_FLAG( flags, SMALLOC_FLAG_PTR_ARRAY );
        }

        smalloc_pointer_t* ptr = NULL;
        smalloc_result_t   res = _i_smalloc_ptr_create_ptr_array( &ptr, size, elem_size, flags );
        if ( SMALLOC_OK != res ) {
                return NULL;
        }
        ctx->stats.total_allocations++;

        if ( SMALLOC_IS_FLAG_SET( flags, SMALLOC_FLAG_PERSIST ) ) {
                _i_smalloc_memlist_add( ctx->alloc_list, ptr );
                ctx->stats.current_allocations_list++;
                return ptr->ptr;
        }

        _i_smalloc_memstack_push( ctx->alloc_stack, ptr );
        ctx->stats.current_allocations_stack++;
        return ptr->ptr;
}

__SMALLOC_API void* smalloc_realloc( smalloc_ctx_t* ctx, void* ptr, size_t new_size ) {
        if ( NULL == ctx ) {
                return NULL;
        }
        if ( NULL == ptr || 0 == new_size ) {
                ctx->last_operation_result = SMALLOC_ERR_INVALID_PARAM;
                return NULL;
        }

        smalloc_pointer_t tmp_ptr = { 0 };
        tmp_ptr.ptr               = ptr;
        size_t           index    = 0;
        smalloc_result_t res      = _i_smalloc_memlist_find( ctx->alloc_list, &tmp_ptr, &index );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }
        smalloc_pointer_t* old_ptr = NULL;
        res = _i_smalloc_memlist_retrieve_index( ctx->alloc_list, &old_ptr, index );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }

        if ( SMALLOC_IS_FLAG_SET( old_ptr->flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                ctx->last_operation_result = SMALLOC_ERR_INVALID_PTR_TYPE;
                return NULL;
        }
        res = _i_smalloc_ptr_realloc_single( &old_ptr, new_size );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }

        ptr                        = NULL;
        ctx->last_operation_result = SMALLOC_OK;
        return old_ptr->ptr;
}

__SMALLOC_API void* smalloc_realloc_arr( smalloc_ctx_t* ctx, void* ptr, size_t new_size ) {
        if ( NULL == ctx ) {
                return NULL;
        }
        if ( NULL == ptr || 0 == new_size ) {
                ctx->last_operation_result = SMALLOC_ERR_INVALID_PARAM;
                return NULL;
        }

        smalloc_pointer_t tmp_ptr = { 0 };
        tmp_ptr.ptr               = ptr;
        size_t           index    = 0;
        smalloc_result_t res      = _i_smalloc_memlist_find( ctx->alloc_list, &tmp_ptr, &index );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }
        smalloc_pointer_t* old_ptr = NULL;
        res = _i_smalloc_memlist_retrieve_index( ctx->alloc_list, &old_ptr, index );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }

        if ( !SMALLOC_IS_FLAG_SET( old_ptr->flags, SMALLOC_FLAG_PTR_ARRAY ) ) {
                ctx->last_operation_result = SMALLOC_ERR_INVALID_PTR_TYPE;
                return NULL;
        }
        res = _i_smalloc_ptr_realloc_array( &old_ptr, new_size );
        if ( SMALLOC_OK != res ) {
                ctx->last_operation_result = res;
                return NULL;
        }

        ptr                        = NULL;
        ctx->last_operation_result = SMALLOC_OK;
        return old_ptr->ptr;
}

__SMALLOC_API void smalloc_add_ptr_to_ctx( smalloc_ctx_t* ctx, void* ptr, const size_t size, uint32_t flags ) { }

__SMALLOC_API void smalloc_free_all( smalloc_ctx_t* ctx ) {
        if ( NULL == ctx ) {
                return;
        }

        smalloc_pointer_t* ptr = NULL;
        while ( !_i_smalloc_memstack_is_empty( ctx->alloc_stack ) ) {
                smalloc_result_t res = _i_smalloc_memstack_pop( ctx->alloc_stack, &ptr );
                if ( SMALLOC_ERR_STACK_EMPTY == res ) {
                        break;
                }
                _i_smalloc_ptr_free( &ptr );
                ctx->stats.current_allocations_stack--;
                ctx->stats.total_allocations--;
                ctx->stats.total_allocations_freed++;
        }

        for ( size_t i = 0; i < ctx->alloc_list->size; i++ ) {
                ptr                  = ctx->alloc_list->items[i];
                smalloc_result_t res = _i_smalloc_memlist_remove( ctx->alloc_list, ptr );
                if ( SMALLOC_ERR_LIST_EMPTY == res ) {
                        break;
                }
                ctx->stats.current_allocations_list--;
                ctx->stats.total_allocations--;
                ctx->stats.total_allocations_freed++;
        }
}

__SMALLOC_API void smalloc_free_stack( smalloc_ctx_t* ctx ) {
        if ( NULL == ctx ) {
                return;
        }

        smalloc_pointer_t* ptr = NULL;
        while ( !_i_smalloc_memstack_is_empty( ctx->alloc_stack ) ) {
                smalloc_result_t res = _i_smalloc_memstack_pop( ctx->alloc_stack, &ptr );
                if ( SMALLOC_ERR_STACK_EMPTY == res ) {
                        break;
                }
                _i_smalloc_ptr_free( &ptr );
                ctx->stats.current_allocations_stack--;
                ctx->stats.total_allocations--;
                ctx->stats.total_allocations_freed++;
        }
}

__SMALLOC_API void smalloc_free_list( smalloc_ctx_t* ctx ) {
        if ( NULL == ctx ) {
                return;
        }

        smalloc_pointer_t* ptr = NULL;
        for ( size_t i = 0; i < ctx->alloc_list->size; i++ ) {
                ptr                  = ctx->alloc_list->items[i];
                smalloc_result_t res = _i_smalloc_memlist_remove( ctx->alloc_list, ptr );
                if ( SMALLOC_ERR_LIST_EMPTY == res ) {
                        break;
                }
                ctx->stats.current_allocations_list--;
                ctx->stats.total_allocations--;
                ctx->stats.total_allocations_freed++;
        }
}

__SMALLOC_API void smalloc_free( smalloc_ctx_t* ctx, void* ptr ) { }
