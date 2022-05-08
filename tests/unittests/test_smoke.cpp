#include <gtest/gtest.h>

#include <string>
namespace
{
TEST(MyString, DefaultConstructor)
{
    std::string s;
    EXPECT_EQ(0u, s.size());
}
}  // namespace
