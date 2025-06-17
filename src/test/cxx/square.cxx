#include <string>
#include <vector>

#include <fmt/format.h>

#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(square,square_v0)
{
    ASSERT_EQ( format( square(v0) ), "000000000000000000000000" );
}

TEST(square,square_v1)
{
    ASSERT_EQ( format( square(v1) ), "000000000000000000000001" );
}

TEST(square,square_v2)
{
    ASSERT_EQ( format( square(v2) ), "000000000000000000000004" );
}

TEST(square,square_vL)
{
    ASSERT_EQ( format( square(vL) ), "000000004000000000000000" );
}

TEST(square,square_vM)
{
    ASSERT_EQ( format( square(vM) ), "00000000FFFFFFFE00000001" );
}

TEST(square,square_v00)
{
    ASSERT_EQ( format( square(v00) ), "0000000000000000000000000000000000000000" );
}

TEST(square,square_v01)
{
    ASSERT_EQ( format( square(v01) ), "0000000000000000000000010000000000000000" );
}

TEST(square,square_v02)
{
    ASSERT_EQ( format( square(v02) ), "0000000000000000000000040000000000000000" );
}

TEST(square,square_v0L)
{
    ASSERT_EQ( format( square(v0L) ), "0000000040000000000000000000000000000000" );
}

TEST(square,square_v0M)
{
    ASSERT_EQ( format( square(v0M) ), "00000000FFFFFFFE000000010000000000000000" );
}
