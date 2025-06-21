// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
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

TEST(square,square_C6D4E3CC)
{
    auto const x = vector { 0xC6D4E3CCU };
    ASSERT_EQ( format( square(x) ), "000000009A6E0169A4936A90" );
}

TEST(square,square_17259613C6D4E3CC)
{
    auto const x = vector { 0xC6D4E3CCU, 0x17259613U };
    ASSERT_EQ( format( square(x) ), "000000000217C67C476CE90119F2E1B1A4936A90" );
}

TEST(square,square_46B67C81FC10D1A7A6B76CE6)
{
    auto const x = vector { 0xA6B76CE6U, 0xFC10D1A7U, 0x46B67C81U };
    ASSERT_EQ( format( square(x) ), "0000000013884E2C5B39C8C215CC2EB14643B120DC7204BB2726DEA4" );
}
