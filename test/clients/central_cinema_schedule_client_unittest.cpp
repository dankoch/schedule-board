#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "file_utils.h"
#include "calendar_event.h"

#include "mock_http_utils.hpp"

#include "central_cinema_schedule_client.h"
#include "config_options.h"

using namespace std::placeholders;
using ::testing::_;
using ::testing::Return;

namespace {

class CentralCinemaScheduleClientTest : public testing::Test {

protected:
  util::MockHttpUtil mockHttpUtil;

  std::shared_ptr<ConfigOptions> _config;
  ScheduleClient centralCinemaScheduleClient;


  virtual void SetUp() {
    /* Build the client instance, populated with our testing configuration. */
    std::shared_ptr<ConfigOptions> _config = std::make_shared<ConfigOptions>();
    _config->setStoragePath("test/tmp");

    centralCinemaScheduleClient = CentralCinemaScheduleClient(_config);

    /* Bind the httpGetToFile method of the mock class to the http fetch function used by the client. */
    centralCinemaScheduleClient.httpGetToFile = std::bind(&MockHttpUtil::httpGetToFile, &mockHttpUtil, _1, _2);
  }

  virtual void TearDown() {
    delete _config;
  }
};


/* Basic test to confirm that a month of data can be fetched, parsed, and returned. */
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarEventsForAMonth) {
  std::string downloadFileDestination;

  EXPECT_CALL(mockHttpUtil, 
    httpGetToFile("https://public.ticketbiscuit.com/CentralCinema/Calendar/2018/11", _))
    .Times(1)
    .DoAll(
      SaveArg<1>(&downloadFileDestination),
      InvokeWithoutArgs([=](){
        util::copyFile("./test/data/clients/central_schedule_201811.html",,downloadFileDestination);
      }),
      Return(0));

  std::tm startDate;
  strptime("2018-11-01 00:00", "%Y-%m-%dT %H:%M", &startDate);

  std::tm endDate;
  strptime("2018-11-30 23:59", "%Y-%m-%dT %H:%M", &startDate);

  vector<CalendarEvent> resultVector = centralCinemaScheduleClient.fetchCalendarEvents(&startDate, &endDate);
  EXPECT_EQ(resultVector.size, 23);
}

/* Test to confirm that nothing tries to get fetched if the start date is after the end date. */
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarEventsInvertedDates) {

}

/* Test to confirm that the right data is excluded for start/end dates within a given month. */
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarEventsWithinMonth) {

}

/* Test to confirm that data can be properly fetched when a request spans year boundaries. */
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarEventsYearBoundaries) {

}

/* Test to confirm that leftover data from previous requests do not effect subsequent pulls.*/
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarEventsDataCleanup) {

}

/* Test to confirm that failed month requests are properly skipped and handled. */
TEST_F(CentralCinemaScheduleClientTest, FetchCalendarFailedRequestTest) {

}
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}