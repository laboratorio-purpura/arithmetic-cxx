#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly,is_compact_vE)
{
    ASSERT_EQ( is_compact(vE), 0 );
}

TEST(poly,is_compact_v0)
{
    ASSERT_EQ( is_compact(v0), 1 );
}

TEST(poly,is_compact_v1)
{
    ASSERT_EQ( is_compact(v1), 1 );
}

TEST(poly,is_compact_v2)
{
    ASSERT_EQ( is_compact(v2), 1 );
}

TEST(poly,is_compact_vL)
{
    ASSERT_EQ( is_compact(vL), 1 );
}

TEST(poly,is_compact_vM)
{
    ASSERT_EQ( is_compact(vM), 1 );
}

TEST(poly,is_compact_v00)
{
    ASSERT_EQ( is_compact(v00), 0 );
}

TEST(poly,is_compact_v10)
{
    ASSERT_EQ( is_compact(v10), 0 );
}

TEST(poly,is_compact_v20)
{
    ASSERT_EQ( is_compact(v20), 0 );
}

TEST(poly,is_compact_vL0)
{
    ASSERT_EQ( is_compact(vL0), 0 );
}

TEST(poly,is_compact_vM0)
{
    ASSERT_EQ( is_compact(vM0), 0 );
}

TEST(poly,is_compact_v01)
{
    ASSERT_EQ( is_compact(v01), 1 );
}

TEST(poly,is_compact_v02)
{
    ASSERT_EQ( is_compact(v02), 1 );
}

TEST(poly,is_compact_v0L)
{
    ASSERT_EQ( is_compact(v0L), 1 );
}

TEST(poly,is_compact_v0M)
{
    ASSERT_EQ( is_compact(v0M), 1 );
}