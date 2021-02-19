
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "CountClient.h"

class MockCountClient : public CountClient
{
public:
    MOCK_METHOD(bool, _setup, (std::string, int), (override));
    MOCK_METHOD(bool, SendMsg, (std::string), (override));
};

TEST(CountClient, CountEverySecond) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, SendMsg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, SendMsg).Times(3);

    EXPECT_EQ(true, client.Setup("AAA", 123));
    EXPECT_EQ(true, client.IsConnected());

    sleep(3);
    client.Shutdown();
    EXPECT_EQ(false, client.IsConnected());
}

TEST(CountClient, StopCountIfInactive) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, SendMsg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, SendMsg).Times(3);

    EXPECT_EQ(true, client.Setup("AAA", 123));
    EXPECT_EQ(true, client.IsConnected());

    sleep(3);
    client.on_recv("inactive");
    sleep(3);

    client.Shutdown();
}

TEST(CountClient, RestartCountIfActive) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, SendMsg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, SendMsg).Times(6);

    EXPECT_EQ(true, client.Setup("AAA", 123));
    EXPECT_EQ(true, client.IsConnected());

    sleep(3);
    client.on_recv("inactive");
    sleep(3);
    client.on_recv("active");
    sleep(3);

    client.Shutdown();
}

TEST(CountClient, AddCountIfAck) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, SendMsg).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(client, SendMsg("3"));

    EXPECT_EQ(true, client.Setup("AAA", 123));
    EXPECT_EQ(true, client.IsConnected());

    client.on_recv("ack");
    client.on_recv("ack");
    client.on_recv("ack");
    sleep(1);

    client.Shutdown();
}

TEST(CountClient, ShutdownIfReceiveShutdown) {
    MockCountClient client;
    EXPECT_CALL(client, _setup).WillOnce(testing::Return(true));
    EXPECT_CALL(client, SendMsg).WillRepeatedly(testing::Return(true));

    EXPECT_EQ(true, client.Setup("AAA", 123));
    EXPECT_EQ(true, client.IsConnected());

    sleep(1);
    client.on_recv("shutdown");
    sleep(1);
    EXPECT_EQ(false, client.IsConnected());
}
