#ifndef SMALLOC_SMALLOC_GUARDS_H
#define SMALLOC_SMALLOC_GUARDS_H

#if defined( _WIN32 ) || defined( __CYGWIN__ )
        // Windows is not supported just yet because who cares
  #ifdef SMALLOC_EXPORTS
    #define __SMALLOC_API __declspec( dllexport )
  #else
    #define __SMALLOC_API __declspec( dllimport )
  #endif
#else
  #ifdef SMALLOC_EXPORTS
    #define __SMALLOC_API             __attribute__( ( visibility( "default" ) ) )
    #define __SMALLOC_ALLOCATOR_API   __attribute__( ( visibility( "default" ), warn_unused_result ) )

    #define __SMALLOC_DEALLOCATOR_API __attribute__( ( visibility( "default" ) ) )
    #define __SMALLOC_INTERNAL        __attribute__( ( visibility( "hidden" ) ) )
  #else
    #define __SMALLOC_API
    #define __SMALLOC_ALLOCATOR_API
    #define __SMALLOC_DEALLOCATOR_API
    #define __SMALLOC_INTERNAL
  #endif
#endif

#endif // SMALLOC_SMALLOC_GUARDS_H
