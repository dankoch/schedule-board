# Enable Testing
enable_testing()

# Test and test-relevant files.
set(SCHEDULE_BOARD_TEST_SOURCES
      ${CMAKE_CURRENT_LIST_DIR}/clients/central_cinema_schedule_client_unittest.cpp
      ${CMAKE_CURRENT_LIST_DIR}/mock/mock_http_utils.hpp
)

# Add the necessary sources and include directories to the test target.
target_sources(scheduleBoardTest
    PRIVATE
      ${SCHEDULE_BOARD_TEST_SOURCES}
)

target_include_directories(ScheduleBoardTest 
                              ${CMAKE_CURRENT_LIST_DIR}
                              ${CMAKE_CURRENT_LIST_DIR}/mock
                              ${CMAKE_CURRENT_LIST_DIR}/clients
)

# Register the tests.
gtest_discover_tests(
    TARGET ScheduleBoardTest
    SOURCES ${SCHEDULE_BOARD_TEST_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DISCOVERY_TIMEOUT 30
)