
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "CountServer.h"

class MockCountServer : public CountServer
{
public:
    MOCK_METHOD(void, BroadcastMsg, (std::string), (override));
    MOCK_METHOD(void, SendMsg, (int, std::string), (override));
};

TEST(CountServer, CountServer) {
    CountServer server;
    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}

TEST(CountServer, on_recv_normal_count) {
    MockCountServer server;
    EXPECT_CALL(server, SendMsg(1, std::string("ack"))).Times(1);

    server.on_recv(1, std::string("1"));

    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}

TEST(CountServer, on_recv_prime_count) {
    MockCountServer server;
    EXPECT_CALL(server, SendMsg(1, std::string("ack"))).Times(1);

    server.on_recv(1, std::string("3"));

    EXPECT_EQ(1, server.get_state());
    EXPECT_EQ(6, server.get_timeout());
}

TEST(CountServer, on_recv_reject) {
    MockCountServer server;
    EXPECT_CALL(server, SendMsg(1, std::string("ack"))).Times(1);
    EXPECT_CALL(server, SendMsg(2, std::string("inactive"))).Times(1);

    server.on_recv(1, std::string("3"));
    server.on_recv(2, std::string("3"));

    EXPECT_EQ(1, server.get_state());
    EXPECT_EQ(6, server.get_timeout());
}

TEST(CountServer, on_recv_prime_count_recover) {
    MockCountServer server;
    EXPECT_CALL(server, SendMsg(1, std::string("ack"))).Times(2);
    EXPECT_CALL(server, BroadcastMsg(std::string("active"))).Times(1);

    server.on_recv(1, std::string("3"));
    server.on_recv(1, std::string("6"));

    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}