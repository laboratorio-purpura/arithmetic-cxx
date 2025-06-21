// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly_remainder,remainder_v0_1)
{
    ASSERT_EQ( format( remainder(v0,1u) ), "00000000" );
}

TEST(poly_remainder,remainder_v1_1)
{
    ASSERT_EQ( format( remainder(v1,1u) ), "00000000" );
}

TEST(poly_remainder,remainder_v2_1)
{
    ASSERT_EQ( format( remainder(v2,1u) ), "00000000" );
}

TEST(poly_remainder,remainder_vL_1)
{
    ASSERT_EQ( format( remainder(vL,1u) ), "00000000" );
}

TEST(poly_remainder,remainder_vM_1)
{
    ASSERT_EQ( format( remainder(vM,1u) ), "00000000" );
}

TEST(poly_remainder,remainder_v01_1)
{
    ASSERT_EQ( format( remainder(v01,1u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v02_1)
{
    ASSERT_EQ( format( remainder(v02,1u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v0L_1)
{
    ASSERT_EQ( format( remainder(v0L,1u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v0M_1)
{
    ASSERT_EQ( format( remainder(v0M,1u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v1_2)
{
    ASSERT_EQ( format( remainder(v1,2u) ), "00000001" );
}

TEST(poly_remainder,remainder_v2_2)
{
    ASSERT_EQ( format( remainder(v2,2u) ), "00000000" );
}

TEST(poly_remainder,remainder_vL_2)
{
    ASSERT_EQ( format( remainder(vL,2u) ), "00000000" );
}

TEST(poly_remainder,remainder_vM_2)
{
    ASSERT_EQ( format( remainder(vM,2u) ), "00000001" );
}

TEST(poly_remainder,remainder_v01_2)
{
    ASSERT_EQ( format( remainder(v01,2u) ), "0000000100000000" );
}

TEST(poly_remainder,remainder_v02_2)
{
    ASSERT_EQ( format( remainder(v02,2u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v0L_2)
{
    ASSERT_EQ( format( remainder(v0L,2u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_v0M_2)
{
    ASSERT_EQ( format( remainder(v0M,2u) ), "0000000100000000" );
}

TEST(poly_remainder,remainder_v0_3)
{
    ASSERT_EQ( format( remainder(v0,3u) ), "00000000" );
}

TEST(poly_remainder,remainder_v1_3)
{
    ASSERT_EQ( format( remainder(v1,3u) ), "00000001" );
}

TEST(poly_remainder,remainder_v2_3)
{
    ASSERT_EQ( format( remainder(v2,3u) ), "00000002" );
}

TEST(poly_remainder,remainder_vL_3)
{
    ASSERT_EQ( format( remainder(vL,3u) ), "00000002" );
}

TEST(poly_remainder,remainder_vM_3)
{
    ASSERT_EQ( format( remainder(vM,3u) ), "00000000" );
}

TEST(poly_remainder,remainder_v01_3)
{
    ASSERT_EQ( format( remainder(v01,3u) ), "0000000100000000" );
}

TEST(poly_remainder,remainder_v02_3)
{
    ASSERT_EQ( format( remainder(v02,3u) ), "0000000200000000" );
}

TEST(poly_remainder,remainder_v0L_3)
{
    ASSERT_EQ( format( remainder(v0L,3u) ), "0000000200000000" );
}

TEST(poly_remainder,remainder_v0M_3)
{
    ASSERT_EQ( format( remainder(v0M,3u) ), "0000000000000000" );
}

TEST(poly_remainder,remainder_43F509DFA4E23125_CD35EC77)
{
    vector<unsigned> x { 0xA4E23125, 0x43F509DF };
    unsigned y { 0xCD35EC77 };
    ASSERT_EQ( format( remainder(x,y) ), "A60B677E" );
}
