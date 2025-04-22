#include <smalloc/smalloc.h>
#include <stdio.h>

int main( void ) {
        smalloc_ctx_t*   ctx = NULL;
        smalloc_result_t res = smalloc_init_ctx( &ctx );
        if ( res != SMALLOC_OK ) {
                return -1;
        }
        char*  character       = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character2      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character3      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character4      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character5      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character6      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character7      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character8      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character9      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character10     = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );

        char** character_array = smalloc_alloc_arr( 10, sizeof( char ), ctx, SMALLOC_FLAG_PERSIST );

        printf( "Current allocations stack: %zu\n", ctx->stats.current_allocations_stack );
        printf( "Current allocations list: %zu\n", ctx->stats.current_allocations_list );
        printf( "Total allocations: %zu\n", ctx->stats.total_allocations );
        printf( "Total allocations freed: %zu\n", ctx->stats.total_allocations_freed );

        smalloc_free_all( ctx );

        printf( "Current allocations stack: %zu\n", ctx->stats.current_allocations_stack );
        printf( "Current allocations list: %zu\n", ctx->stats.current_allocations_list );
        printf( "Total allocations: %zu\n", ctx->stats.total_allocations );
        printf( "Total allocations freed: %zu\n", ctx->stats.total_allocations_freed );
        smalloc_free_destroy_ctx( &ctx );

        return 0;
}

