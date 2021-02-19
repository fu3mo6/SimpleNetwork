
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "CountClient.h"

class MockCountClient : public CountClient
{
public:
    MOCK_METHOD(bool, _setup, (std::string, int), (override));
    MOCK_METHOD(bool, send_msg, (std::string), (override));
};

TEST(CountClient, CountEverySecond) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, send_msg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, send_msg).Times(3);

    EXPECT_EQ(true, client.setup("AAA", 123));
    EXPECT_EQ(true, client.is_connected());

    sleep(3);
    client.do_shutdown();
    EXPECT_EQ(false, client.is_connected());
}

TEST(CountClient, StopCountIfInactive) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, send_msg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, send_msg).Times(3);

    EXPECT_EQ(true, client.setup("AAA", 123));
    EXPECT_EQ(true, client.is_connected());

    sleep(3);
    client.on_recv("inactive");
    sleep(3);

    client.do_shutdown();
}

TEST(CountClient, RestartCountIfActive) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, send_msg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, send_msg).Times(6);

    EXPECT_EQ(true, client.setup("AAA", 123));
    EXPECT_EQ(true, client.is_connected());

    sleep(3);
    client.on_recv("inactive");
    sleep(3);
    client.on_recv("active");
    sleep(3);

    client.do_shutdown();
}

TEST(CountClient, AddCountIfAck) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, send_msg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, send_msg("3"));

    EXPECT_EQ(true, client.setup("AAA", 123));
    EXPECT_EQ(true, client.is_connected());

    client.on_recv("ack");
    client.on_recv("ack");
    client.on_recv("ack");
    sleep(1);

    client.do_shutdown();
}

TEST(CountClient, ShutdownIfReceiveShutdown) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, send_msg).WillRepeatedly(testing::Return(true));

    EXPECT_EQ(true, client.setup("AAA", 123));
    EXPECT_EQ(true, client.is_connected());

    sleep(1);
    client.on_recv("shutdown");
    sleep(1);
    EXPECT_EQ(false, client.is_connected());
}
