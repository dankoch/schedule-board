# CMake generated Testfile for 
# Source directory: /Users/dkoch/development/schedule-board
# Build directory: /Users/dkoch/development/schedule-board
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(CentralCinemaScheduleClientTest.FetchCalendarEventsForAMonth "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarEventsForAMonth")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarEventsForAMonth PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
add_test(CentralCinemaScheduleClientTest.FetchCalendarEventsInvertedDates "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarEventsInvertedDates")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarEventsInvertedDates PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
add_test(CentralCinemaScheduleClientTest.FetchCalendarEventsWithinMonth "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarEventsWithinMonth")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarEventsWithinMonth PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
add_test(CentralCinemaScheduleClientTest.FetchCalendarEventsYearBoundaries "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarEventsYearBoundaries")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarEventsYearBoundaries PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
add_test(CentralCinemaScheduleClientTest.FetchCalendarEventsDataCleanup "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarEventsDataCleanup")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarEventsDataCleanup PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
add_test(CentralCinemaScheduleClientTest.FetchCalendarFailedRequestTest "/Users/dkoch/development/schedule-board/build/scheduleBoardTest" "--gtest_filter=CentralCinemaScheduleClientTest.FetchCalendarFailedRequestTest")
set_tests_properties(CentralCinemaScheduleClientTest.FetchCalendarFailedRequestTest PROPERTIES  WORKING_DIRECTORY "/Users/dkoch/development/schedule-board/test")
subdirs("lib/json")
subdirs("lib/spdlog")
subdirs("lib/googletest")
