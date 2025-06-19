#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(mono,is_zero_0)
{
    ASSERT_EQ( is_zero(0U), 1 );
}

TEST(mono,is_zero_1)
{
    ASSERT_EQ( is_zero(1U), 0 );
}

TEST(mono,is_zero_2)
{
    ASSERT_EQ( is_zero(2U), 0 );
}

TEST(mono,is_zero_L)
{
    ASSERT_EQ( is_zero(L), 0 );
}

TEST(mono,is_zero_M)
{
    ASSERT_EQ( is_zero(M), 0 );
}