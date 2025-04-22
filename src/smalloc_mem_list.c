#define __SMALLOC_INTERNAL_FUNCTIONS

#include "smalloc/internal/smalloc_mem_list.h"

smalloc_result_t _i_smalloc_memlist_init( struct smalloc_list_t** list, const size_t init_cap ) {
        if ( ( *list ) != NULL ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        *list = (struct smalloc_list_t*) malloc( sizeof( struct smalloc_list_t ) );
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

smalloc_result_t _i_smalloc_memlist_destroy( struct smalloc_list_t** list ) {
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

smalloc_result_t _i_smalloc_memlist_add( struct smalloc_list_t* list, smalloc_pointer_t* ptr ) {
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

smalloc_result_t _i_smalloc_memlist_find( const struct smalloc_list_t* list,
                                          smalloc_pointer_t*           ptr,
                                          size_t*                      index ) {
        if ( NULL == list || NULL == ptr ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( 0 == list->size ) {
                return SMALLOC_ERR_LIST_EMPTY;
        }

        for ( size_t i = 0; i < list->size; i++ ) {
                if ( list->items[i] == ptr ) {
                        if ( index != NULL ) {
                                *index = i;
                        }
                        return SMALLOC_OK;
                }
        }
        return SMALLOC_ERR_PTR_NOT_FOUND;
}

smalloc_result_t _i_smalloc_memlist_remove( struct smalloc_list_t* list, smalloc_pointer_t* ptr ) {
        if ( NULL == list || NULL == ptr ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( 0 == list->size ) {
                return SMALLOC_ERR_LIST_EMPTY;
        }

        for ( size_t i = 0; i < list->size; i++ ) {
                if ( list->items[i]->ptr == ptr->ptr ) {
                        _i_smalloc_ptr_free( &list->items[i] );
                        // TODO: Possibly half the capacity if size * 2 < capacity
                        for ( size_t j = i; j < list->size - 1; j++ ) {
                                list->items[j] = list->items[j + 1];
                        }
                        list->size--;
                        return SMALLOC_OK;
                }
        }
        return SMALLOC_ERR_PTR_NOT_FOUND;
}

smalloc_result_t _i_smalloc_memlist_free( struct smalloc_list_t* list ) {
        if ( NULL == list ) {
                return SMALLOC_ERR_INVALID_PARAM;
        }

        if ( 0 == list->size ) {
                return SMALLOC_ERR_LIST_EMPTY;
        }

        for ( size_t i = 0; i < list->size; i++ ) {
                free( list->items[i] );
                list->items[i] = NULL;
        }
        list->size = 0;

        return SMALLOC_OK;
}

bool _i_smalloc_memlist_is_empty( const struct smalloc_list_t* list ) {
        if ( NULL == list ) {
                return true;
        }
        if ( 0 == list->size ) {
                return true;
        }
        return false;
}
