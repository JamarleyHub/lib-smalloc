#ifndef SMALLOC_SMALLOC_RESULT_T_H
#define SMALLOC_SMALLOC_RESULT_T_H

#ifdef __cplusplus
extern "C"
{
#endif

        typedef enum
        {
                SMALLOC_OK,

                SMALLOC_ERR_GENERIC,
                SMALLOC_ERR_INVALID_PARAM,
                SMALLOC_ERR_STACK_NOT_EMPTY,
                SMALLOC_ERR_LIST_NOT_EMPTY,
                SMALLOC_ERR_OUT_OF_MEMORY,
                SMALLOC_ERR_STACK_EMPTY,
                SMALLOC_ERR_LIST_EMPTY,
                SMALLOC_ERR_PTR_NOT_FOUND,
                SMALLOC_ERR_WRONG_PTR_CREATE_CALL,
                SMALLOC_ERR_NO_PTR_FOUND,
                SMALLOC_ERR_INVALID_PTR_TYPE,
                SMALLOC_ERR_SHRINK_FAILED,
        } smalloc_result_t;

        /**
         * Converts a smalloc result to a string
         * @param result The smalloc result to convert
         * @return The string representation of the smalloc result
         */
        char* smalloc_result_to_str( smalloc_result_t result );

#ifdef __cplusplus
}
#endif
#endif // SMALLOC_SMALLOC_RESULT_T_H
