cmake_minimum_required(VERSION 3.30)
project(smalloc VERSION 1.0.0 LANGUAGES C)

include(CMakePackageConfigHelpers)

# Create the library
add_library(smalloc SHARED
        src/smalloc.c
        src/smalloc_mem_list.c
        src/smalloc_mem_stack.c
)

# Set the library version properties
set_target_properties(smalloc PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Define symbol visibility for shared library
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # Add symbol visibility flags for GCC/Clang
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
    # Define the export macro
    target_compile_definitions(smalloc PRIVATE SMALLOC_EXPORTS)
endif ()

# Set include directories for the build
target_include_directories(smalloc
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smalloc/internal
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Installation rules
install(TARGETS smalloc
        EXPORT smalloc-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

# Install only the public header
install(FILES include/smalloc/smalloc.h
        DESTINATION include
)

# Export targets
install(EXPORT smalloc-targets
        FILE smalloc-targets.cmake
        NAMESPACE smalloc::
        DESTINATION lib/cmake/smalloc
)

# Generate the package configuration file
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/smallocConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/smallocConfig.cmake
        INSTALL_DESTINATION lib/cmake/smalloc
)

# Generate the package version file
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/smallocConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

# Install the config files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/smallocConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/smallocConfigVersion.cmake
        DESTINATION lib/cmake/smalloc
)