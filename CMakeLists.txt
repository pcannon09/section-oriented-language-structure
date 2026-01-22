# PCANNON CMAKELISTS.TXT v1.2S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250913
# https://github.com/pcannon09/pcannonProjectStandards

cmake_minimum_required(VERSION 3.24)

# Link exec if only changed
cmake_policy(SET CMP0147 NEW)

include(CMakePackageConfigHelpers)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.private/project.json" projectInfo)
string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

# Version configuration
set(VERSION "${projectVersion}")
set(VERSION_STATE "${projectVersionState}")
set(VERSION_STD "${projectVersionSTD}")

if (WIN32)
	set(__SQT_USER_HOME_DIR "$ENV{USERPROFILE}")
else()
	set(__SQT_USER_HOME_DIR "$ENV{HOME}")
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

add_definitions(-DDEFAULT_PYTHON_PATH="${Python3_EXECUTABLE}")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_source_info.cmake")

set(SOLS_USER_HOME_DIR ${__SOLS_USER_HOME_DIR})

project(${projectName} VERSION ${projectVersion} LANGUAGES CXX C)

# Build mode configuration
option(DEV_MODE "Enable development mode" ON)
set(BUILD_MODE "Development" CACHE STRING "Build mode")

if(NOT DEV_MODE)
    set(DEV_MODE OFF)
    set(BUILD_MODE "Production")
endif()

if(DEV_MODE)
    set(BUILD_MODE "Development" CACHE STRING "Build mode")
else()
    set(BUILD_MODE "Production" CACHE STRING "Build mode")
endif()

if(NOT CMAKE_BUILD_TYPE)
    if(DEV_MODE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    else()
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    endif()
endif()

# Build Configuration

if (NOT WIN32)
    set(DEV_EXTRA_COMPILELINK_FLAGS
        -fsanitize=address,leak
    )
endif()

# Development mode flags
set(DEV_COMPILE_FLAGS
    ${DEV_EXTRA_COMPILELINK_FLAGS}

    -fno-omit-frame-pointer
    -g
    -Wunused
    -Wall
    -Wno-range-loop-analysis
    -DSOLS_DEV=true
)

set(DEV_LINK_FLAGS
    ${DEV_EXTRA_COMPILELINK_FLAGS}
    -g
)

# Production mode flags
set(PROD_COMPILE_FLAGS
    -O3
    -DNDEBUG
    -DSOLS_DEV=false
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
set(CPP_STD 20)
set(CMAKE_CXX_STANDARD ${CPP_STD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(C_STD 11)
set(CMAKE_C_STANDARD ${C_STD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_AUTOGEN_VERBOSE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_subdirectory(${VENDOR_DIR}/ciof ciof)
add_subdirectory(${VENDOR_DIR}/cstr cstr)
add_subdirectory(${VENDOR_DIR}/cvec cvec)

include_directories(${VENDOR_DIR}/ciof/inc/)
include_directories(${VENDOR_DIR}/cvec/inc/)
include_directories(${VENDOR_DIR}/cstr/inc/)

add_executable(${projectName} ${SOLS_SOURCES})

# Build Targets
function(configure_lib libSuffix libType)
	add_library(${projectName}_${libSuffix} ${libType}
			${SOLS_SOURCES}
	)

	add_library(${projectName}_lib_${libSuffix} ${libType}
			${SOLS_LIB_HEADERS_LIBS}
			${SOLS_LIB_SOURCES_LIBS}
	)

	add_library(${projectName}_core_${libSuffix} ${libType}
			${SOLS_CORE_HEADERS_LIBS}
			${SOLS_CORE_SOURCES_LIBS}
	)
endfunction()

function(configure_lib libSuffix libType)
    # Core library
    add_library(${projectName}_core_${libSuffix} ${libType}
        ${SOLS_CORE_HEADERS_LIBS}
        ${SOLS_CORE_SOURCES_LIBS}
    )

    # Mark DLL export macro for Windows
    if(WIN32 AND libType STREQUAL "SHARED")
        target_compile_definitions(${projectName}_core_${libSuffix} PRIVATE SOLS_BUILD_DLL)
    endif()

    # Lib library
    add_library(${projectName}_lib_${libSuffix} ${libType}
        ${SOLS_LIB_HEADERS_LIBS}
        ${SOLS_LIB_SOURCES_LIBS}
    )

    # Mark DLL export macro for Windows
    if(WIN32 AND libType STREQUAL "SHARED")
        target_compile_definitions(${projectName}_lib_${libSuffix} PRIVATE SOLS_BUILD_DLL)
    endif()

    target_link_libraries(${projectName}_lib_${libSuffix}
        PRIVATE
            ${projectName}_core_${libSuffix}

            cstr_${libSuffix}
            cvec_${libSuffix}
            ciof_${libSuffix}
    )

    # Main library
    add_library(${projectName}_${libSuffix} ${libType}
        ${SOLS_SOURCES}
    )

    if(WIN32 AND libType STREQUAL "SHARED")
        target_compile_definitions(${projectName}_${libSuffix} PRIVATE SOLS_BUILD_DLL)
    endif()

    target_link_libraries(${projectName}_core_${libSuffix}
        PRIVATE
            cstr_${libSuffix}
            ciof_${libSuffix}
            cvec_${libSuffix}
    )

    target_link_libraries(${projectName}_${libSuffix}
        PRIVATE
            ${projectName}_lib_${libSuffix}
            ${projectName}_core_${libSuffix}
    )
endfunction()

function(configure_target tgt libType)
    target_compile_options(${tgt} PRIVATE ${PROGRAM_COMPILE_FLAGS})
    target_link_options(${tgt} PRIVATE ${PROGRAM_LINK_FLAGS})

    target_compile_definitions(${tgt} PRIVATE SOLS_USR_HOME_DIR="${SOLS_USER_HOME_DIR}")

    # Link only the main shared / static library; all sub-deps are internal
    target_link_libraries(${tgt} PRIVATE ${projectName}_${libType}
        cstr_${libType}
    )

    message(STATUS "[${libType}] Linked ${projectName}_${libType} to ${tgt}")
endfunction()

configure_lib(static STATIC)
# configure_lib(shared SHARED)

configure_target(${projectName} static)
# configure_target(${projectName} shared)

# Output directory
set_target_properties(
    ${projectName}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# INSTALL / EXPORT CONFIGURATION
# Install libraries
# install(TARGETS ${projectName}_static ${projectName}_shared
#     EXPORT ${projectName}Target
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin
#     INCLUDES DESTINATION include
# )

# # Install headers
# install(DIRECTORY ${INC_DIR}/ ${SRC_DIR}/
#     DESTINATION include/${projectName}
# )

# install(EXPORT ${projectName}Target
#     FILE ${projectName}Target.cmake
#     NAMESPACE SOLS::
#     DESTINATION lib/cmake/${projectName}
# )

# # Export package

# # Generate config file
# configure_package_config_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_package_config.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
#     INSTALL_DESTINATION lib/cmake/${projectName}
# )

# # Install package config files
# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
#     DESTINATION lib/cmake/${projectName}
# )

# # UNINSTALL
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#     @ONLY
# )

# add_custom_target(sols_uninstall
#     COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#     COMMENT "Uninstalling ${projectName} ${VERSION}-${VERSION_STATE}..."
# )

# # Generate a version file
# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
#     VERSION ${VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

include(CPack)

# PACKAGE CONFIG
set(CPACK_PACKAGE_NAME ${projectName})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "") # TODO:
set(CPACK_PACKAGE_VENDOR "pcannon")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# BUILD INFO
message(STATUS "Project: ${projectName}")
message(STATUS "Version: ${VERSION}-${VERSION_STATE}")
message(STATUS "Standard: ${VERSION_STD}")
message(STATUS "Build Mode: ${BUILD_MODE}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Standard: C++${CPP_STD}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Standard: C${C_STD}")

# Usage instructions
message(STATUS "Usage:")
message(STATUS "  Development build: cmake -DDEV_MODE=ON ..")
message(STATUS "  Production build:  cmake -DDEV_MODE=OFF ..")
message(STATUS "  Install:           ./compile.sh install {PATH}")
message(STATUS "  Uninstall:         ./compile.sh uninstall")
