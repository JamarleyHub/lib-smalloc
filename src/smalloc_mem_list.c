#include "smalloc/internal/smalloc_mem_list.h"

smalloc_result_t _i_smalloc_memlist_init( smalloc_list_t** list, const size_t init_cap ) {
        if ( ( *list ) != NULL ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        *list = (smalloc_list_t*) malloc( sizeof( smalloc_list_t ) );
        if ( NULL == ( *list ) ) {
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        if ( init_cap < _I_SMALLOC_INITIAL_LIST_CAPACITY ) {
                ( *list )->capacity = _I_SMALLOC_INITIAL_LIST_CAPACITY;
        } else {
                ( *list )->capacity = init_cap;
        }

        ( *list )->items = (smalloc_pointer_t**) malloc( sizeof( smalloc_pointer_t ) * init_cap );
        if ( NULL == ( *list )->items ) {
                ( *list )->capacity = 0;
                ( *list )->size     = 0;
                free( *list );
                ( *list ) = NULL;
                return SMALLOC_ERR_OUT_OF_MEMORY;
        }

        ( *list )->size = 0;
        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memlist_destroy( smalloc_list_t** list ) {
        if ( NULL == ( *list ) ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }
        if ( ( *list )->size > 0 ) {
                return SMALLOC_ERR_LIST_NOT_EMPTY;
        }

        if ( ( *list )->items != NULL ) {
                free( ( *list )->items );
                ( *list )->items = NULL;
        }

        ( *list )->capacity = 0;
        ( *list )->size     = 0;

        free( *list );
        *list = NULL;

        return SMALLOC_OK;
}

smalloc_result_t _i_smalloc_memlist_add( smalloc_list_t* list, smalloc_pointer_t* ptr ) {
        if ( NULL == list || NULL == ptr ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( list->size >= list->capacity ) {
                size_t              new_capacity = list->capacity * 2;
                smalloc_pointer_t** new_items =
                    (smalloc_pointer_t**) realloc( list->items,
                                                   sizeof( smalloc_pointer_t* ) * new_capacity );
                if ( NULL == new_items ) {
                        return SMALLOC_ERR_OUT_OF_MEMORY;
                }
                list->items    = new_items;
                list->capacity = new_capacity;
        }
        list->items[list->size] = ptr;
        list->size++;
        return SMALLOC_OK;
}
