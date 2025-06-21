// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.math;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly,not_zero_vE)
{
    ASSERT_EQ( not_zero(vE), 0 );
}

TEST(poly,not_zero_v0)
{
    ASSERT_EQ( not_zero(v0), 0 );
}

TEST(poly,not_zero_v1)
{
    ASSERT_EQ( not_zero(v1), 1 );
}

TEST(poly,not_zero_v2)
{
    ASSERT_EQ( not_zero(v2), 1 );
}

TEST(poly,not_zero_vL)
{
    ASSERT_EQ( not_zero(vL), 1 );
}

TEST(poly,not_zero_vM)
{
    ASSERT_EQ( not_zero(vM), 1 );
}

TEST(poly,not_zero_v00)
{
    ASSERT_EQ( not_zero(v00), 0 );
}

TEST(poly,not_zero_v10)
{
    ASSERT_EQ( not_zero(v10), 1 );
}

TEST(poly,not_zero_v20)
{
    ASSERT_EQ( not_zero(v20), 1 );
}

TEST(poly,not_zero_vL0)
{
    ASSERT_EQ( not_zero(vL0), 1 );
}

TEST(poly,not_zero_vM0)
{
    ASSERT_EQ( not_zero(vM0), 1 );
}

TEST(poly,not_zero_v01)
{
    ASSERT_EQ( not_zero(v01), 1 );
}

TEST(poly,not_zero_v02)
{
    ASSERT_EQ( not_zero(v02), 1 );
}

TEST(poly,not_zero_v0L)
{
    ASSERT_EQ( not_zero(v0L), 1 );
}

TEST(poly,not_zero_v0M)
{
    ASSERT_EQ( not_zero(v0M), 1 );
}