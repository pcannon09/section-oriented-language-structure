# PCANNON CMAKELISTS.TXT v1.1S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards
# NOTE:
# This CMake file is to be announced as a v1.2S version

cmake_minimum_required(VERSION 3.24)

# Link exec if only changed
cmake_policy(SET CMP0147 NEW)

include(CMakePackageConfigHelpers)

# Build mode configuration
option(DEV_MODE "Enable development mode" ON)
set(BUILD_MODE "Development" CACHE STRING "Build mode")

if(NOT DEV_MODE)
    set(DEV_MODE ON)
    set(BUILD_MODE "Development")
endif()

if(DEV_MODE)
    set(BUILD_MODE "Development" CACHE STRING "Build mode")
else()
    set(BUILD_MODE "Production" CACHE STRING "Build mode")
endif()

# Read metadata
file(READ ".private/project.json" projectInfo)
string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

# Version configuration
set(VERSION "${projectVersion}")
set(VERSION_STATE "${projectVersionState}")
set(VERSION_STD "${projectVersionSTD}")

project(${projectName} VERSION ${projectVersion} LANGUAGES CXX)

# Build Configuration

# Development mode flags
set(DEV_COMPILE_FLAGS
    # -fsanitize=address,leak
    -g
    -Wunused
    -Wall
    -Wno-range-loop-analysis
    -DPTREE_DEV=true
)

set(DEV_LINK_FLAGS
    # -fsanitize=address,leak
    -g
)

# Production mode flags
set(PROD_COMPILE_FLAGS
    -O3
    -DNDEBUG
    -DPTREE_DEV=false
)

set(PROD_LINK_FLAGS
    -O3
)

# Apply flags based on mode
if(DEV_MODE)
    set(PROGRAM_COMPILE_FLAGS ${DEV_COMPILE_FLAGS})
    set(PROGRAM_LINK_FLAGS ${DEV_LINK_FLAGS})
else()
    set(PROGRAM_COMPILE_FLAGS ${PROD_COMPILE_FLAGS})
    set(PROGRAM_LINK_FLAGS ${PROD_LINK_FLAGS})
endif()

# Project Configuration
set(CPP_STD 17)
set(CMAKE_CXX_STANDARD ${CPP_STD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOGEN_VERBOSE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Directory Setup
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/${projectName}")

# Source Files
set(HEADERS
    ${INC_DIR}/CIOFpredefines.hpp

    ${INC_DIR}/ciof.hpp
    ${INC_DIR}/ciof.ipp
)

set(SOURCE_NON_INSTALL ${TESTS_DIR}/main.cpp)
set(SOURCE_INSTALLED ${TESTS_DIR}/releaseMain.cpp)
set(SOURCE_NON_INSTALL_VER_CHECK ${TESTS_DIR}/version.cpp)

set(SOURCES
    ${SRC_DIR}/ciof.cpp
)

# Build Targets
add_library(${projectName}_static STATIC ${SOURCES} ${HEADERS})
add_library(${projectName}_shared SHARED ${SOURCES} ${HEADERS})

add_executable(${projectName}_non_install ${SOURCE_NON_INSTALL})
add_executable(${projectName}_installed ${SOURCE_INSTALLED})
add_executable(${projectName}_version_check ${SOURCE_NON_INSTALL_VER_CHECK})

function(configure_target tgt)
    target_compile_options(${tgt} PRIVATE ${PROGRAM_COMPILE_FLAGS})
    target_link_options(${tgt} PRIVATE ${PROGRAM_LINK_FLAGS})
    target_include_directories(${tgt} PRIVATE ${INC_DIR})
    target_link_libraries(${tgt} PRIVATE ${projectName}_static)
endfunction()

configure_target(${projectName}_non_install)
configure_target(${projectName}_installed)
configure_target(${projectName}_version_check)

# Output directory
set_target_properties(
    ${projectName}_non_install
    ${projectName}_installed
    ${projectName}_version_check
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# INSTALL / EXPORT CONFIGURATION
# Install libraries
install(TARGETS ${projectName}_static ${projectName}_shared
    EXPORT ${projectName}Target
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY ${INC_DIR}/ ${SRC_DIR}/
    DESTINATION include/${projectName}
)

install(EXPORT ${projectName}Target
    FILE ${projectName}Target.cmake
    NAMESPACE Ciof::
    DESTINATION lib/cmake/${projectName}
)

# Export package

# Generate config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_package_config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${projectName}
)

# Install package config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
    DESTINATION lib/cmake/${projectName}
)

# UNINSTALL
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)

add_custom_target(ciof_uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    COMMENT "Uninstalling ${projectName} ${VERSION}-${VERSION_STATE}..."
)

# Generate a version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY AnyNewerVersion
)

# PACKAGE CONFIG
set(CPACK_PACKAGE_NAME ${projectName})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Generated by pcannon Project Standards")
set(CPACK_PACKAGE_VENDOR "pcannon")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)

# BUILD INFO
message(STATUS "Project: ${projectName}")
message(STATUS "Version: ${VERSION}-${VERSION_STATE}")
message(STATUS "Standard: ${VERSION_STD}")
message(STATUS "Build Mode: ${BUILD_MODE}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Standard: C++${CPP_STD}")

# Usage instructions
message(STATUS "Usage:")
message(STATUS "  Development build: cmake -DDEV_MODE=ON ..")
message(STATUS "  Production build:  cmake -DDEV_MODE=OFF ..")
message(STATUS "  Install:           ./compile.sh install {PATH}")
message(STATUS "  Uninstall:         ./compile.sh uninstall")

