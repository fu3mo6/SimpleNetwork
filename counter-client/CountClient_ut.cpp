
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "CountClient.h"

/*
class MockCountServer : public CountServer
{
public:
    MOCK_METHOD(void, broadcast_msg, (std::string), (override));
    MOCK_METHOD(void, send_msg, (int, std::string), (override));
};

TEST(CountServer, CountServer) {
    CountServer server;
    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}

TEST(CountServer, on_recv_normal_count) {
    MockCountServer server;
    EXPECT_CALL(server, send_msg(1, std::string("ack"))).Times(1);

    server.on_recv(1, std::string("1"));

    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}

TEST(CountServer, on_recv_prime_count) {
    MockCountServer server;
    EXPECT_CALL(server, send_msg(1, std::string("ack"))).Times(1);

    server.on_recv(1, std::string("3"));

    EXPECT_EQ(1, server.get_state());
    EXPECT_EQ(6, server.get_timeout());
}

TEST(CountServer, on_recv_reject) {
    MockCountServer server;
    EXPECT_CALL(server, send_msg(1, std::string("ack"))).Times(1);
    EXPECT_CALL(server, send_msg(2, std::string("inactive"))).Times(1);

    server.on_recv(1, std::string("3"));
    server.on_recv(2, std::string("3"));

    EXPECT_EQ(1, server.get_state());
    EXPECT_EQ(6, server.get_timeout());
}

TEST(CountServer, on_recv_prime_count_recover) {
    MockCountServer server;
    EXPECT_CALL(server, send_msg(1, std::string("ack"))).Times(2);
    EXPECT_CALL(server, broadcast_msg(std::string("active"))).Times(1);

    server.on_recv(1, std::string("3"));
    server.on_recv(1, std::string("6"));

    EXPECT_EQ(-1, server.get_state());
    EXPECT_EQ(-1, server.get_timeout());
}*/