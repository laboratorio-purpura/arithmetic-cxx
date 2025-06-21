// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(sum,sum_vM_1)
{
    ASSERT_EQ( format( sum(vM,1U) ), "0000000100000000" );
}

TEST(sum,sum_v0_v0)
{
    ASSERT_EQ( format( sum(v0,v0) ), "0000000000000000" );
}

TEST(sum,sum_v0_v1)
{
    ASSERT_EQ( format( sum(v0,v1) ), "0000000000000001" );
}

TEST(sum,sum_v0_v2)
{
    ASSERT_EQ( format( sum(v0,v2) ), "0000000000000002" );
}

TEST(sum,sum_v0_vL)
{
    ASSERT_EQ( format( sum(v0,vL) ), "0000000080000000" );
}

TEST(sum,sum_v0_vM)
{
    ASSERT_EQ( format( sum(v0,vM) ), "00000000FFFFFFFF" );
}

TEST(sum,sum_v1_v0)
{
    ASSERT_EQ( format( sum(v1,v0) ), "0000000000000001" );
}

TEST(sum,sum_v1_v1)
{
    ASSERT_EQ( format( sum(v1,v1) ), "0000000000000002" );
}

TEST(sum,sum_v1_v2)
{
    ASSERT_EQ( format( sum(v1,v2) ), "0000000000000003" );
}

TEST(sum,sum_v1_vL)
{
    ASSERT_EQ( format( sum(v1,vL) ), "0000000080000001" );
}

TEST(sum,sum_v1_vM)
{
    ASSERT_EQ( format( sum(v1,vM) ), "0000000100000000" );
}

TEST(sum,sum_vL_v0)
{
    ASSERT_EQ( format( sum(vL,v0) ), "0000000080000000" );
}

TEST(sum,sum_vL_v1)
{
    ASSERT_EQ( format( sum(vL,v1) ), "0000000080000001" );
}

TEST(sum,sum_vL_v2)
{
    ASSERT_EQ( format( sum(vL,v2) ), "0000000080000002" );
}

TEST(sum,sum_vL_vL)
{
    ASSERT_EQ( format( sum(vL,vL) ), "0000000100000000" );
}

TEST(sum,sum_vL_vM)
{
    ASSERT_EQ( format( sum(vL,vM) ), "000000017FFFFFFF" );
}

TEST(sum,sum_vM_v0)
{
    ASSERT_EQ( format( sum(vM,v0) ), "00000000FFFFFFFF" );
}

TEST(sum,sum_vM_v1)
{
    ASSERT_EQ( format( sum(vM,v1) ), "0000000100000000" );
}

TEST(sum,sum_vM_v2)
{
    ASSERT_EQ( format( sum(vM,v2) ), "0000000100000001" );
}

TEST(sum,sum_vM_vL)
{
    ASSERT_EQ( format( sum(vM,vL) ), "000000017FFFFFFF" );
}

TEST(sum,sum_vM_vM)
{
    ASSERT_EQ( format( sum(vM,vM) ), "00000001FFFFFFFE" );
}

TEST(sum,sum_v01_v0)
{
    ASSERT_EQ( format( sum(v01,v0) ), "000000000000000100000000" );
}

TEST(sum,sum_v01_v01)
{
    ASSERT_EQ( format( sum(v01,v01) ), "000000000000000200000000" );
}

TEST(sum,sum_v01_v02)
{
    ASSERT_EQ( format( sum(v01,v02) ), "000000000000000300000000" );
}

TEST(sum,sum_v01_v0L)
{
    ASSERT_EQ( format( sum(v01,v0L) ), "000000008000000100000000" );
}

TEST(sum,sum_v01_v0M)
{
    ASSERT_EQ( format( sum(v01,v0M) ), "000000010000000000000000" );
}

TEST(sum,sum_v0L_v0)
{
    ASSERT_EQ( format( sum(v0L,v0) ), "000000008000000000000000" );
}

TEST(sum,sum_v0L_v01)
{
    ASSERT_EQ( format( sum(v0L,v01) ), "000000008000000100000000" );
}

TEST(sum,sum_v0L_v02)
{
    ASSERT_EQ( format( sum(v0L,v02) ), "000000008000000200000000" );
}

TEST(sum,sum_v0L_v0L)
{
    ASSERT_EQ( format( sum(v0L,v0L) ), "000000010000000000000000" );
}

TEST(sum,sum_v0L_v0M)
{
    ASSERT_EQ( format( sum(v0L,v0M) ), "000000017FFFFFFF00000000" );
}

TEST(sum,sum_v0M_v0)
{
    ASSERT_EQ( format( sum(v0M,v0) ), "00000000FFFFFFFF00000000" );
}

TEST(sum,sum_v0M_v01)
{
    ASSERT_EQ( format( sum(v0M,v01) ), "000000010000000000000000" );
}

TEST(sum,sum_v0M_v02)
{
    ASSERT_EQ( format( sum(v0M,v02) ), "000000010000000100000000" );
}

TEST(sum,sum_v0M_v0L)
{
    ASSERT_EQ( format( sum(v0M,v0L) ), "000000017FFFFFFF00000000" );
}

TEST(sum,sum_v0M_v0M)
{
    ASSERT_EQ( format( sum(v0M,v0M) ), "00000001FFFFFFFE00000000" );
}
