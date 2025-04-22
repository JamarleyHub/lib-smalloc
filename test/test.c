// Include the smalloc library
#include <smalloc/smalloc.h>
#include <stdio.h>

void print_current_allocations( const smalloc_ctx_t* ctx ) {
        // The context has a stats field which contains the number of current allocations
        printf( "Current allocations:\n" );
        printf( "    Current allocations stack: %zu\n", ctx->stats.current_allocations_stack );
        printf( "    Current allocations list: %zu\n", ctx->stats.current_allocations_list );
        printf( "    Total allocations: %zu\n", ctx->stats.total_allocations );
        printf( "    Total allocations freed: %zu\n", ctx->stats.total_allocations_freed );
        printf( "-------------------------\n\n" );
}

int main( void ) {
        // Initialize the smalloc context
        smalloc_ctx_t*   ctx = NULL;
        smalloc_result_t res = smalloc_init_ctx( &ctx );
        // Check the return status!
        if ( res != SMALLOC_OK ) {
                return -1;
        }

        // Now some simple allocations with the context
        char*  character       = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character2      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character3      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character4      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        char*  character5      = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );

        // This is an array allocation which allocates 10 char*
        // The SMALLOC_FLAG_PERSIST means this can individually be free'd
        char** character_array = smalloc_alloc_arr( 10, sizeof( char ), ctx, SMALLOC_FLAG_PERSIST );

        print_current_allocations( ctx );

        // Reallocation also works
        character       = smalloc_realloc( ctx, character, sizeof( char ) * 2 );
        character_array = smalloc_realloc_arr( ctx, character_array, sizeof( char ) * 20 );

        // Checking if the last operation was a success should also be done:
        if ( ctx->last_operation_result != SMALLOC_OK ) {
                printf( "Reallocation of the array failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        // Forgot how big the array was? No problem, we can get the size of the pointer
        size_t size = smalloc_get_ptr_size( ctx, character_array );
        printf( "    Size of character_array: %zu\n\n", size );

        // Let's assume we already got a malloc'd pointer and want to manage it with smalloc
        // Note: This may give you a memory leak warning by your IDE because static analyzers
        // aren't the greatest. Don't worry, this is expected. Your memory is NOT in fact leaking.
        // Don't believe me? Run the build with valgrind and check the log.
        int* int_array = malloc( sizeof( int ) * 10 );

        // We can add it to the context with smalloc_add_ptr_to_ctx
        smalloc_add_ptr_to_ctx( ctx, int_array, sizeof( int ) * 10, SMALLOC_FLAG_PERSIST );

        print_current_allocations( ctx );

        // Once we are done, we can free all pointers in the context like this
        smalloc_free_all( ctx );

        print_current_allocations( ctx );

        // Or we can free them and then automatically destroy the context.
        smalloc_free_destroy_ctx( &ctx );

        return 0;
}

