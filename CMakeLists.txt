cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

set(PRO_NAME "bulk_server")

project(${PRO_NAME} VERSION ${PROJECT_VERSION})

add_library(async SHARED 
    src/async.cpp
    src/commands.cpp
    src/parser.cpp
    src/shared_commands.cpp
)

target_include_directories(async PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
    include/commands
    include/lexer
    include/output
    include/parser
)

configure_file(version.h.in version.h)
find_package(Boost CONFIG COMPONENTS program_options REQUIRED)

add_executable(${PRO_NAME} src/main.cpp)

target_link_libraries(${PRO_NAME} async Boost::program_options)

target_include_directories(${PRO_NAME} PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
    include/commands
    include/lexer
    include/output
    include/parser
)
  
set_target_properties(async ${PRO_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PRO_NAME} PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PRO_NAME} PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()

#include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
#configure_package_config_file(
#  ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
#  "${CMAKE_CURRENT_BINARY_DIR}/${PRO_NAME}-config.cmake"
#  INSTALL_DESTINATION "lib/cmake/${PRO_NAME}"
#  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
#)

# generate the version file for the config file
#write_basic_package_version_file(
#  "${CMAKE_CURRENT_BINARY_DIR}/${PRO_NAME}-config-version.cmake"
#  VERSION "${PROJECT_VERSION}"
#  COMPATIBILITY AnyNewerVersion
#)

# install the configuration file
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PRO_NAME}-config.cmake
#              ${CMAKE_CURRENT_BINARY_DIR}/${PRO_NAME}-config-version.cmake
#        DESTINATION lib/cmake/${PRO_NAME}
#)

install(TARGETS async ${PRO_NAME})

# install header files
#install(DIRECTORY include/
#        DESTINATION include/${PRO_NAME}
#        FILES_MATCHING PATTERN "*.h"
#        )

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

