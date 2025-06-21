// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(twice,twice_v0)
{
    ASSERT_EQ( format( twice(v0) ), "0000000000000000" );
}

TEST(twice,twice_v1)
{
    ASSERT_EQ( format( twice(v1) ), "0000000000000002" );
}

TEST(twice,twice_v2)
{
    ASSERT_EQ( format( twice(v2) ), "0000000000000004" );
}

TEST(twice,twice_vL)
{
    ASSERT_EQ( format( twice(vL) ), "0000000100000000" );
}

TEST(twice,twice_vM)
{
    ASSERT_EQ( format( twice(vM) ), "00000001FFFFFFFE" );
}

TEST(twice,twice_v01)
{
    ASSERT_EQ( format( twice(v01) ), "000000000000000200000000" );
}

TEST(twice,twice_v02)
{
    ASSERT_EQ( format( twice(v02) ), "000000000000000400000000" );
}

TEST(twice,twice_v0L)
{
    ASSERT_EQ( format( twice(v0L) ), "000000010000000000000000" );
}

TEST(twice,twice_v0M)
{
    ASSERT_EQ( format( twice(v0M) ), "00000001FFFFFFFE00000000" );
}
