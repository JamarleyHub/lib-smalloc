#define __SMALLOC_INTERNAL_FUNCTIONS
#include "smalloc/internal/smalloc_mem_stack.h"

smalloc_result_t _i_smalloc_memstack_init( struct smalloc_stack_t** stack, const size_t init_cap ) {
        if ( ( *stack ) != NULL ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        *stack = (struct smalloc_stack_t*) malloc( sizeof( struct smalloc_stack_t ) );
        if ( NULL == ( *stack ) ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }
        if ( init_cap < _I_SMALLOC_INITIAL_STACK_CAPACITY ) {
                ( *stack )->capacity = _I_SMALLOC_INITIAL_STACK_CAPACITY;
        } else {
                ( *stack )->capacity = init_cap;
        }
        ( *stack )->items = (smalloc_pointer_t**) malloc( sizeof( void* ) * init_cap );
        if ( NULL == ( *stack )->items ) {
                ( *stack )->capacity = 0;
                ( *stack )->size     = 0;
                free( *stack );
                ( *stack ) = NULL;
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *stack )->size = 0;
        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memstack_destroy( struct smalloc_stack_t** stack ) {
        if ( NULL == ( *stack ) ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( ( *stack )->size > 0 ) {
                return SMALLOC_ERR_STACK_NOT_EMPTY;
        }

        if ( ( *stack )->items != NULL ) {
                free( ( *stack )->items );
                ( *stack )->items = NULL;
        }

        ( *stack )->capacity = 0;
        ( *stack )->size     = 0;

        free( *stack );
        *stack = NULL;

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memstack_push( struct smalloc_stack_t* stack, smalloc_pointer_t* ptr ) {
        if ( NULL == stack || NULL == ptr ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        // Check if we need to grow the stack
        if ( stack->size >= stack->capacity ) {
                size_t              new_capacity = stack->capacity * 2;
                smalloc_pointer_t** new_items =
                    (smalloc_pointer_t**) realloc( stack->items,
                                                   sizeof( smalloc_pointer_t* ) * new_capacity );

                if ( NULL == new_items ) {
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }

                stack->items    = new_items;
                stack->capacity = new_capacity;
        }

        stack->items[stack->size] = ptr;
        stack->size++;
        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memstack_pop( struct smalloc_stack_t* stack, smalloc_pointer_t** ptr ) {
        if ( NULL == stack ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        // Check if the stack is empty
        if ( 0 == stack->size ) {
                *ptr = NULL;
                return SMALLOC_ERR_STACK_EMPTY;
        }

        stack->size--;
        *ptr = stack->items[stack->size];

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memstack_peek( const struct smalloc_stack_t* stack,
                                           smalloc_pointer_t**           ptr ) {
        if ( NULL == stack ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        // Check if the stack is empty
        if ( 0 == stack->size ) {
                *ptr = NULL;
                return SMALLOC_ERR_STACK_EMPTY;
        }

        *ptr = stack->items[stack->size - 1];

        return SMALLOC_OK;
}

bool _i_smalloc_memstack_is_empty( const struct smalloc_stack_t* stack ) {
        if ( NULL == stack ) {
                return true;
        }
        if ( 0 == stack->size ) {
                return true;
        }
        return false;
}
