// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.math;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly,is_zero_vE)
{
    ASSERT_EQ( is_zero(vE), 1 );
}

TEST(poly,is_zero_v0)
{
    ASSERT_EQ( is_zero(v0), 1 );
}

TEST(poly,is_zero_v1)
{
    ASSERT_EQ( is_zero(v1), 0 );
}

TEST(poly,is_zero_v2)
{
    ASSERT_EQ( is_zero(v2), 0 );
}

TEST(poly,is_zero_vL)
{
    ASSERT_EQ( is_zero(vL), 0 );
}

TEST(poly,is_zero_vM)
{
    ASSERT_EQ( is_zero(vM), 0 );
}

TEST(poly,is_zero_v00)
{
    ASSERT_EQ( is_zero(v00), 1 );
}

TEST(poly,is_zero_v10)
{
    ASSERT_EQ( is_zero(v10), 0 );
}

TEST(poly,is_zero_v20)
{
    ASSERT_EQ( is_zero(v20), 0 );
}

TEST(poly,is_zero_vL0)
{
    ASSERT_EQ( is_zero(vL0), 0 );
}

TEST(poly,is_zero_vM0)
{
    ASSERT_EQ( is_zero(vM0), 0 );
}

TEST(poly,is_zero_v01)
{
    ASSERT_EQ( is_zero(v01), 0 );
}

TEST(poly,is_zero_v02)
{
    ASSERT_EQ( is_zero(v02), 0 );
}

TEST(poly,is_zero_v0L)
{
    ASSERT_EQ( is_zero(v0L), 0 );
}

TEST(poly,is_zero_v0M)
{
    ASSERT_EQ( is_zero(v0M), 0 );
}