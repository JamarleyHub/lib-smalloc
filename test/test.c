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
        /*******************************************************************************
         *                                  Basic setup                                *
         *******************************************************************************/

        // Initialize the smalloc context
        smalloc_ctx_t*   ctx = NULL;
        smalloc_result_t res = smalloc_init_ctx( &ctx );
        // Check the return status!
        if ( res != SMALLOC_OK ) {
                return -1;
        }

        /*******************************************************************************
         *                               Basic allocations                             *
         *******************************************************************************/

        // Now some simple allocations with the context
        char* character = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );

        // Checking if the last operation was a success should also be done:
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Allocation of character failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        char* character2 = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_AUTO );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Allocation of character2 failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        // This is a dynamic allocation which can be free'd earlier than the end of the function
        char* character3 = smalloc_alloc( sizeof( char ), ctx, SMALLOC_FLAG_DYNAMIC );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Allocation of character3 failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        // This is an array allocation which allocates 10 char*
        char** character_array = smalloc_alloc_arr( 10, sizeof( char ), ctx, SMALLOC_FLAG_DYNAMIC );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Allocation of character_array failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        /*******************************************************************************
         *                                 Reallocations                               *
         *******************************************************************************/

        // Reallocation also works
        character       = smalloc_realloc( ctx, character, sizeof( char ) * 2 );
        character_array = smalloc_realloc_arr( ctx, character_array, sizeof( char ) * 20 );
        if ( ctx->last_operation_result != SMALLOC_OK ) {
                printf( "Reallocation of the array failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        /*******************************************************************************
         *                                Utility functions                            *
         *******************************************************************************/

        // Forgot how big the array was? No problem, we can get the size of the pointer
        size_t size = smalloc_get_ptr_size( ctx, character_array );
        printf( " - Size of character_array: %zu\n\n", size );

        // Let's assume we already got a malloc'd pointer and want to manage it with smalloc
        // Note: This may give you a memory leak warning by your IDE because static analyzers
        // aren't the greatest. Don't worry, this is expected. Your memory is NOT in fact leaking.
        // Don't believe me? Run the build with valgrind and check the log.
        int* int_array = malloc( sizeof( int ) * 10 );
        if ( NULL == int_array ) {
                printf( "Allocation of int_array failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        // We can add it to the context with smalloc_add_ptr_to_ctx
        smalloc_add_ptr_to_ctx( ctx, int_array, sizeof( int ) * 10, SMALLOC_FLAG_DYNAMIC );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Adding pointer to ctx failed!\n" );
                free( int_array );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        // This also works for 2D arrays, although these won't have an element size then, it's your
        // job to manage that
        float** float_array = malloc( sizeof( float* ) * 5 );
        if ( NULL == float_array ) {
                printf( "Allocation of float_array failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }
        for ( size_t i = 0; i < 5; i++ ) {
                float_array[i] = malloc( sizeof( float ) * 10 );
                if ( NULL == float_array[i] ) {
                        printf( "Allocation of float_array[%zu] failed!\n", i );
                        for ( size_t j = 0; j < i; j++ ) {
                                free( float_array[j] );
                        }
                        free( float_array );
                        smalloc_free_destroy_ctx( &ctx );
                        return -1;
                }
        }
        smalloc_add_ptr_to_ctx( ctx, float_array, 5, SMALLOC_FLAG_PTR_ARRAY );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Adding pointer to ctx failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        /*******************************************************************************
         *                                 Freeing Memory                              *
         *******************************************************************************/

        // What if I want to free a pointer individually? If it was allocated using FLAG_DYNAMIC,
        // we can free it like this:
        smalloc_free( ctx, character3 );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Freeing individual pointer failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        // Don't want smalloc to free a pointer automatically anymore? There's a flag for that.
        // First we need to set the appropriate flag - Keep in mind that we cannot suddenly
        // make our pointer DYNAMIC if it wasn't before. This operation currently only allows
        // to set an additional persistence flag!
        smalloc_set_flags( ctx, character2, ( SMALLOC_FLAG_AUTO | SMALLOC_FLAG_PERSISTENT ) );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Setting flags failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }
        // This pointer will now NOT be freed automatically anymore, but we can still free
        // it manually

        // Once we are done, we can free all pointers in the context like this
        smalloc_free_all( ctx );
        if ( SMALLOC_OK != ctx->last_operation_result ) {
                printf( "Freeing all pointers failed!\n" );
                smalloc_free_destroy_ctx( &ctx );
                return -1;
        }

        print_current_allocations( ctx );

        // The context will show the memory as being free'd, but it only means the context does not
        // carry the pointer any longer.
        // To truly free it, we will need to do it ourselves now.
        // You can check this for yourself using Valgrind.
        free( character2 );

        // Or we can free them and then automatically destroy the context.
        smalloc_free_destroy_ctx( &ctx );

        return 0;
}

