cmake_minimum_required(VERSION 3.30)
set(CMAKE_C_COMPILER clang)
project(smalloc VERSION 1.1.0 LANGUAGES C)

include(CMakePackageConfigHelpers)

# Create the library
add_library(smalloc SHARED
        src/smalloc.c
        src/smalloc_mem_list.c
        src/smalloc_mem_stack.c
        src/smalloc_pointer_t.c
        src/smalloc_result_t.c
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

# IMPORTANT: Generate an export header instead of manual defines
include(GenerateExportHeader)
generate_export_header(smalloc
        BASE_NAME SMALLOC
        EXPORT_MACRO_NAME SMALLOC_EXPORT
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/smalloc/smalloc_export.h
        STATIC_DEFINE SMALLOC_STATIC)

# Set include directories for the build - INCLUDE THE GENERATED HEADERS
target_include_directories(smalloc
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smalloc/internal
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Installation rules
install(TARGETS smalloc
        EXPORT smalloc-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
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