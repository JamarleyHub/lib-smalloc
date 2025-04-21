#include "smalloc/internal/smalloc_result_t.h"

char* smalloc_result_to_str( const smalloc_result_t result ) {
        switch ( result ) {
                case SMALLOC_OK :
                        return "SMALLOC_OK";
                case SMALLOC_ERR_GENERIC :
                        return "SMALLOC_ERR_GENERIC";
                case SMALLOC_ERR_INVALID_PARAM :
                        return "SMALLOC_ERR_INVALID_PARAM";
                case SMALLOC_ERR_STACK_NOT_EMPTY :
                        return "SMALLOC_ERR_STACK_NOT_EMPTY";
                case SMALLOC_ERR_LIST_NOT_EMPTY :
                        return "SMALLOC_ERR_LIST_NOT_EMPTY";
                case SMALLOC_ERR_OUT_OF_MEMORY :
                        return "SMALLOC_ERR_OUT_OF_MEMORY";
                case SMALLOC_ERR_STACK_EMPTY :
                        return "SMALLOC_ERR_STACK_EMPTY";
                case SMALLOC_ERR_LIST_EMPTY :
                        return "SMALLOC_ERR_LIST_EMPTY";
                case SMALLOC_ERR_PTR_NOT_FOUND :
                        return "SMALLOC_ERR_PTR_NOT_FOUND";
                case SMALLOC_ERR_WRONG_PTR_CREATE_CALL :
                        return "SMALLOC_ERR_WRONG_PTR_CREATE_CALL";
                case SMALLOC_ERR_NO_PTR_FOUND :
                        return "SMALLOC_ERR_NO_PTR_FOUND";
                case SMALLOC_ERR_INVALID_PTR_TYPE :
                        return "SMALLOC_ERR_INVALID_PTR_TYPE";
                case SMALLOC_ERR_SHRINK_FAILED :
                        return "SMALLOC_ERR_SHRINK_FAILED";
                default :
                        return "UNKNOWN_ERROR_CODE";
        }
}
