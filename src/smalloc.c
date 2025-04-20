#define __SMALLOC_INTERNAL_FUNCTIONS

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
        ( *ctx )->stats                 = (smalloc_stats_t) { 0 };
        ( *ctx )->last_operation_result = SMALLOC_OK;
        return SMALLOC_OK;
}
