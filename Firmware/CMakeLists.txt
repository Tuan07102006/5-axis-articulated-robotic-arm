
cmake_minimum_required(VERSION 3.22)

# ============================================================
# 5-AXIS ARTICULATED ROBOTIC ARM
# STM32F411 - CMake Build System
# ============================================================

# ------------------------------------------------------------
# 1. COMPILER CONFIGURATION
# ------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# ------------------------------------------------------------
# 2. PROJECT CONFIGURATION
# ------------------------------------------------------------

set(CMAKE_PROJECT_NAME 5-axis_articulated_robotic_arm)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${CMAKE_PROJECT_NAME} LANGUAGES C CXX ASM)

message(STATUS "========================================")
message(STATUS "PROJECT : ${CMAKE_PROJECT_NAME}")
message(STATUS "BUILD   : ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")

# ------------------------------------------------------------
# 3. MAIN EXECUTABLE
# ------------------------------------------------------------

add_executable(${CMAKE_PROJECT_NAME})

# STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# ------------------------------------------------------------
# 4. AUTOMATIC MODULE DISCOVERY
# ------------------------------------------------------------

set(MODULE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Modules")

# Automatically detect C and C++ source files
file(GLOB_RECURSE MODULE_SOURCES
    CONFIGURE_DEPENDS
    "${MODULE_ROOT}/*.c"
    "${MODULE_ROOT}/*.cpp"
)

# Automatically detect header files
file(GLOB_RECURSE MODULE_HEADERS
    CONFIGURE_DEPENDS
    "${MODULE_ROOT}/*.h"
    "${MODULE_ROOT}/*.hpp"
)

# Collect header directories
set(MODULE_INCLUDE_DIRS "")

foreach(HEADER_FILE IN LISTS MODULE_HEADERS)
    get_filename_component(
        HEADER_DIR
        "${HEADER_FILE}"
        DIRECTORY
    )

    list(APPEND MODULE_INCLUDE_DIRS "${HEADER_DIR}")
endforeach()

# Remove duplicate include directories
list(REMOVE_DUPLICATES MODULE_INCLUDE_DIRS)

# Register module source files
if(MODULE_SOURCES)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ${MODULE_SOURCES}
    )
endif()

# Register module include directories
if(MODULE_INCLUDE_DIRS)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${MODULE_INCLUDE_DIRS}
    )
endif()

# ------------------------------------------------------------
# 5. ADDITIONAL PROJECT CONFIGURATION
# ------------------------------------------------------------

target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # User library directories
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # User-defined compile definitions
)

# ------------------------------------------------------------
# 6. LINK LIBRARIES
# ------------------------------------------------------------

# Remove incorrect implicit dependency
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    stm32cubemx
)

# ------------------------------------------------------------
# 7. BUILD INFORMATION
# ------------------------------------------------------------

list(LENGTH MODULE_SOURCES MODULE_SOURCE_COUNT)
list(LENGTH MODULE_INCLUDE_DIRS MODULE_INCLUDE_COUNT)

message(STATUS "----------------------------------------")
message(STATUS "Module source files : ${MODULE_SOURCE_COUNT}")
message(STATUS "Module include dirs : ${MODULE_INCLUDE_COUNT}")

foreach(SOURCE_FILE IN LISTS MODULE_SOURCES)
    file(RELATIVE_PATH REL_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${SOURCE_FILE}"
    )

    message(STATUS "  [SOURCE] ${REL_SOURCE}")
endforeach()

message(STATUS "----------------------------------------")

# ------------------------------------------------------------
# 8. OPTIONAL FIRMWARE SIZE INFORMATION
# ------------------------------------------------------------

find_program(ARM_SIZE_TOOL
    NAMES arm-none-eabi-size
)

if(ARM_SIZE_TOOL)
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND "${ARM_SIZE_TOOL}"
                "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
        COMMENT "Checking firmware memory usage"
        VERBATIM
    )
endif()
