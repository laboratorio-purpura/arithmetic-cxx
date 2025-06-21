// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly_quotient,quotient_v0_1)
{
    ASSERT_EQ( format( quotient(v0,1u) ), "00000000" );
}

TEST(poly_quotient,quotient_v1_1)
{
    ASSERT_EQ( format( quotient(v1,1u) ), "00000001" );
}

TEST(poly_quotient,quotient_v2_1)
{
    ASSERT_EQ( format( quotient(v2,1u) ), "00000002" );
}

TEST(poly_quotient,quotient_vL_1)
{
    ASSERT_EQ( format( quotient(vL,1u) ), "80000000" );
}

TEST(poly_quotient,quotient_vM_1)
{
    ASSERT_EQ( format( quotient(vM,1u) ), "FFFFFFFF" );
}

TEST(poly_quotient,quotient_v01_1)
{
    ASSERT_EQ( format( quotient(v01,1u) ), "0000000100000000" );
}

TEST(poly_quotient,quotient_v02_1)
{
    ASSERT_EQ( format( quotient(v02,1u) ), "0000000200000000" );
}

TEST(poly_quotient,quotient_v0L_1)
{
    ASSERT_EQ( format( quotient(v0L,1u) ), "8000000000000000" );
}

TEST(poly_quotient,quotient_v0M_1)
{
    ASSERT_EQ( format( quotient(v0M,1u) ), "FFFFFFFF00000000" );
}

TEST(poly_quotient,quotient_v1_2)
{
    ASSERT_EQ( format( quotient(v1,2u) ), "00000000" );
}

TEST(poly_quotient,quotient_v2_2)
{
    ASSERT_EQ( format( quotient(v2,2u) ), "00000001" );
}

TEST(poly_quotient,quotient_vL_2)
{
    ASSERT_EQ( format( quotient(vL,2u) ), "40000000" );
}

TEST(poly_quotient,quotient_vM_2)
{
    ASSERT_EQ( format( quotient(vM,2u) ), "7FFFFFFF" );
}

TEST(poly_quotient,quotient_v01_2)
{
    ASSERT_EQ( format( quotient(v01,2u) ), "0000000000000000" );
}

TEST(poly_quotient,quotient_v02_2)
{
    ASSERT_EQ( format( quotient(v02,2u) ), "0000000100000000" );
}

TEST(poly_quotient,quotient_v0L_2)
{
    ASSERT_EQ( format( quotient(v0L,2u) ), "4000000000000000" );
}

TEST(poly_quotient,quotient_v0M_2)
{
    ASSERT_EQ( format( quotient(v0M,2u) ), "7FFFFFFF00000000" );
}

TEST(poly_quotient,quotient_v0_3)
{
    ASSERT_EQ( format( quotient(v0,3u) ), "00000000" );
}

TEST(poly_quotient,quotient_v1_3)
{
    ASSERT_EQ( format( quotient(v1,3u) ), "00000000" );
}

TEST(poly_quotient,quotient_v2_3)
{
    ASSERT_EQ( format( quotient(v2,3u) ), "00000000" );
}

TEST(poly_quotient,quotient_vL_3)
{
    ASSERT_EQ( format( quotient(vL,3u) ), "2AAAAAAA" );
}

TEST(poly_quotient,quotient_vM_3)
{
    ASSERT_EQ( format( quotient(vM,3u) ), "55555555" );
}

TEST(poly_quotient,quotient_v01_3)
{
    ASSERT_EQ( format( quotient(v01,3u) ), "0000000000000000" );
}

TEST(poly_quotient,quotient_v02_3)
{
    ASSERT_EQ( format( quotient(v02,3u) ), "0000000000000000" );
}

TEST(poly_quotient,quotient_v0L_3)
{
    ASSERT_EQ( format( quotient(v0L,3u) ), "2AAAAAAA00000000" );
}

TEST(poly_quotient,quotient_v0M_3)
{
    ASSERT_EQ( format( quotient(v0M,3u) ), "5555555500000000" );
}
