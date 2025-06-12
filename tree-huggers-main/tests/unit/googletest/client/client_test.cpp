#include "client.h"
#include "gtest/gtest.h"
#include <string>

TEST(CLIENT_TEST, RETURN_NAME) {
    Client yourclient("Sam");
    std::string name = yourclient.get_name();

    EXPECT_EQ("Sam", name);
}
