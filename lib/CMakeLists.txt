#---------------------------------------------------------------------------------------
# INCLUDE CXXOPTS LIBRARY
#---------------------------------------------------------------------------------------

set(cxxopts_dir ${CMAKE_CURRENT_LIST_DIR}/cxxopts)

# CXXOPTS is all one file – include it as a source and a header.
target_sources(scheduleBoard
    PUBLIC
        "${cxxopts_dir}/include/cxxopts.hpp"
)
target_include_directories(scheduleBoardLib PUBLIC "${cxxopts_dir}/include")


#---------------------------------------------------------------------------------------
# INCLUDE NLOHMANN/JSON LIBRARY
#---------------------------------------------------------------------------------------

set(json_dir ${CMAKE_CURRENT_LIST_DIR}/json)

# Disable unit tests.
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Pull in the CMake subdirectory and link.
add_subdirectory("${json_dir}")
target_link_libraries(scheduleBoardLib PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(scheduleBoardLib PUBLIC "${json_dir}/include")


#---------------------------------------------------------------------------------------
# INCLUDE GUMBO LIBRARY
#---------------------------------------------------------------------------------------

set(gumbo_dir ${CMAKE_CURRENT_LIST_DIR}/gumbo-parser)

# Accrue Gumbo source files.
set(gumbo_SRCS
    ${gumbo_dir}/src/attribute.c
    ${gumbo_dir}/src/attribute.h
    ${gumbo_dir}/src/char_ref.c
    ${gumbo_dir}/src/char_ref.h
    ${gumbo_dir}/src/error.c
    ${gumbo_dir}/src/error.h
    ${gumbo_dir}/src/insertion_mode.h
    ${gumbo_dir}/src/parser.c
    ${gumbo_dir}/src/parser.h
    ${gumbo_dir}/src/string_buffer.c
    ${gumbo_dir}/src/string_buffer.h
    ${gumbo_dir}/src/string_piece.c
    ${gumbo_dir}/src/string_piece.h
    ${gumbo_dir}/src/tag.c
    ${gumbo_dir}/src/tag_enum.h
    ${gumbo_dir}/src/tag_gperf.h
    ${gumbo_dir}/src/tag_strings.h
    ${gumbo_dir}/src/tag_sizes.h
    ${gumbo_dir}/src/token_type.h
    ${gumbo_dir}/src/tokenizer.c
    ${gumbo_dir}/src/tokenizer.h
    ${gumbo_dir}/src/tokenizer_states.h
    ${gumbo_dir}/src/utf8.c
    ${gumbo_dir}/src/utf8.h
    ${gumbo_dir}/src/util.c
    ${gumbo_dir}/src/util.h
    ${gumbo_dir}/src/vector.c
    ${gumbo_dir}/src/vector.h)

# Give all targets access to Gumbo's header files (Gumbo and others)
include_directories(${gumbo_dir}/src)

# Build it as a library
add_library(gumbo ${gumbo_SRCS})

# Link it into the master target
target_link_libraries(scheduleBoardLib PUBLIC gumbo)


#---------------------------------------------------------------------------------------
# INCLUDE SPDLOG LIBRARY
#---------------------------------------------------------------------------------------

set(spdlog_dir ${CMAKE_CURRENT_LIST_DIR}/spdlog)

# Invoke the SPDLOG process.
add_subdirectory("${spdlog_dir}")

# Reference the headers and library.
target_include_directories(scheduleBoardLib 
    PUBLIC 
        "${spdlog_dir}/include"
)

# Skip tests and other side projects.
SET(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Test SPDLOG" FORCE)
SET(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "SPDLOG Examples" FORCE)
SET(SPDLOG_BUILD_BENCH OFF CACHE BOOL "SPDLOG Bench" FORCE)


#---------------------------------------------------------------------------------------
# INCLUDE GOOGLETEST LIBRARY
#---------------------------------------------------------------------------------------

set(googletest_dir ${CMAKE_CURRENT_LIST_DIR}/googletest)

# Build both GMock and GTest
SET(BUILD_GMOCK ON CACHE BOOL "Build GMOCK and GTEST" FORCE)

# Don't install GTEST.  Don't need to – we've embedded it.
SET(INSTALL_GTEST OFF CACHE BOOL "Install GTest" FORCE)

# Invoke the GOOGLETEST build process.
add_subdirectory("${googletest_dir}")

# Include the correct header paths
include_directories(${googletest_dir}/googletest/include  
	${googletest_dir}/googlemock/include
	${googletest_dir})

# Link against our testing executable.
target_link_libraries(scheduleBoardTest gtest gtest_main gmock gmock_main)


#---------------------------------------------------------------------------------------
# INCLUDE CURL LIBRARY
#---------------------------------------------------------------------------------------

# Use the CMake Modules to find the locally installed packages.
include(FindCURL)

# Include and link CURL
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(scheduleBoardLib PRIVATE ${CURL_LIBRARIES})