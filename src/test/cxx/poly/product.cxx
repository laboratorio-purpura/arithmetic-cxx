// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>
#include <vector>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;

using std::array;
using std::random_device;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;
using std::span;
using std::vector;

TEST(poly,product_v0_0)
{
    ASSERT_EQ( format( product(v0,0U) ), "0000000000000000" );
}

TEST(poly,product_v0_1)
{
    ASSERT_EQ( format( product(v0,1U) ), "0000000000000000" );
}

TEST(poly,product_v0_2)
{
    ASSERT_EQ( format( product(v0,2U) ), "0000000000000000" );
}

TEST(poly,product_v0_L)
{
    ASSERT_EQ( format( product(v0,L) ), "0000000000000000" );
}

TEST(poly,product_v0_M)
{
    ASSERT_EQ( format( product(v0,M) ), "0000000000000000" );
}

TEST(poly,product_v1_0)
{
    ASSERT_EQ( format( product(v1,0U) ), "0000000000000000" );
}

TEST(poly,product_v1_1)
{
    ASSERT_EQ( format( product(v1,1U) ), "0000000000000001" );
}

TEST(poly,product_v1_2)
{
    ASSERT_EQ( format( product(v1,2U) ), "0000000000000002" );
}

TEST(poly,product_v1_L)
{
    ASSERT_EQ( format( product(v1,L) ), "0000000080000000" );
}

TEST(poly,product_v1_M)
{
    ASSERT_EQ( format( product(v1,M) ), "00000000FFFFFFFF" );
}

TEST(poly,product_vL_0)
{
    ASSERT_EQ( format( product(vL,0U) ), "0000000000000000" );
}

TEST(poly,product_vL_1)
{
    ASSERT_EQ( format( product(vL,1U) ), "0000000080000000" );
}

TEST(poly,product_vL_2)
{
    ASSERT_EQ( format( product(vL,2U) ), "0000000100000000" );
}

TEST(poly,product_vL_L)
{
    ASSERT_EQ( format( product(vL,L) ), "4000000000000000" );
}

TEST(poly,product_vL_M)
{
    ASSERT_EQ( format( product(vL,M) ), "7FFFFFFF80000000" );
}

TEST(poly,product_vM_0)
{
    ASSERT_EQ( format( product(vM,0U) ), "0000000000000000" );
}

TEST(poly,product_vM_1)
{
    ASSERT_EQ( format( product(vM,1U) ), "00000000FFFFFFFF" );
}

TEST(poly,product_vM_2)
{
    ASSERT_EQ( format( product(vM,2U) ), "00000001FFFFFFFE" );
}

TEST(poly,product_vM_L)
{
    ASSERT_EQ( format( product(vM,L) ), "7FFFFFFF80000000" );
}

TEST(poly,product_vM_M)
{
    ASSERT_EQ( format( product(vM,M) ), "FFFFFFFE00000001" );
}

TEST(poly,product_v0_v0)
{
    ASSERT_EQ( format( product(v0,v0) ), "000000000000000000000000" );
}

TEST(poly,product_v0_v1)
{
    ASSERT_EQ( format( product(v0,v1) ), "000000000000000000000000" );
}

TEST(poly,product_v0_v2)
{
    ASSERT_EQ( format( product(v0,v2) ), "000000000000000000000000" );
}

TEST(poly,product_v0_vL)
{
    ASSERT_EQ( format( product(v0,vL) ), "000000000000000000000000" );
}

TEST(poly,product_v0_vM)
{
    ASSERT_EQ( format( product(v0,vM) ), "000000000000000000000000" );
}

TEST(poly,product_v1_v0)
{
    ASSERT_EQ( format( product(v1,v0) ), "000000000000000000000000" );
}

TEST(poly,product_v1_v1)
{
    ASSERT_EQ( format( product(v1,v1) ), "000000000000000000000001" );
}

TEST(poly,product_v1_v2)
{
    ASSERT_EQ( format( product(v1,v2) ), "000000000000000000000002" );
}

TEST(poly,product_v1_vL)
{
    ASSERT_EQ( format( product(v1,vL) ), "000000000000000080000000" );
}

TEST(poly,product_v1_vM)
{
    ASSERT_EQ( format( product(v1,vM) ), "0000000000000000FFFFFFFF" );
}

TEST(poly,product_vL_v0)
{
    ASSERT_EQ( format( product(vL,v0) ), "000000000000000000000000" );
}

TEST(poly,product_vL_v1)
{
    ASSERT_EQ( format( product(vL,v1) ), "000000000000000080000000" );
}

TEST(poly,product_vL_v2)
{
    ASSERT_EQ( format( product(vL,v2) ), "000000000000000100000000" );
}

TEST(poly,product_vL_vL)
{
    ASSERT_EQ( format( product(vL,vL) ), "000000004000000000000000" );
}

TEST(poly,product_vL_vM)
{
    ASSERT_EQ( format( product(vL,vM) ), "000000007FFFFFFF80000000" );
}

TEST(poly,product_vM_v0)
{
    ASSERT_EQ( format( product(vM,v0) ), "000000000000000000000000" );
}

TEST(poly,product_vM_v1)
{
    ASSERT_EQ( format( product(vM,v1) ), "0000000000000000FFFFFFFF" );
}

TEST(poly,product_vM_v2)
{
    ASSERT_EQ( format( product(vM,v2) ), "0000000000000001FFFFFFFE" );
}

TEST(poly,product_vM_vL)
{
    ASSERT_EQ( format( product(vM,vL) ), "000000007FFFFFFF80000000" );
}

TEST(poly,product_vM_vM)
{
    ASSERT_EQ( format( product(vM,vM) ), "00000000FFFFFFFE00000001" );
}

TEST(poly,product_v0_v01)
{
    ASSERT_EQ( format( product(v0,v01) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v0_v02)
{
    ASSERT_EQ( format( product(v0,v02) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v0_v0L)
{
    ASSERT_EQ( format( product(v0,v0L) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v0_v0M)
{
    ASSERT_EQ( format( product(v0,v0M) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v01_v0)
{
    ASSERT_EQ( format( product(v01,v0) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v01_v01)
{
    ASSERT_EQ( format( product(v01,v01) ), "0000000000000000000000010000000000000000" );
}

TEST(poly,product_v01_v02)
{
    ASSERT_EQ( format( product(v01,v02) ), "0000000000000000000000020000000000000000" );
}

TEST(poly,product_v01_v0L)
{
    ASSERT_EQ( format( product(v01,v0L) ), "0000000000000000800000000000000000000000" );
}

TEST(poly,product_v01_v0M)
{
    ASSERT_EQ( format( product(v01,v0M) ), "0000000000000000FFFFFFFF0000000000000000" );
}

TEST(poly,product_v0L_v0)
{
    ASSERT_EQ( format( product(v0L,v0) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v0L_v01)
{
    ASSERT_EQ( format( product(v0L,v01) ), "0000000000000000800000000000000000000000" );
}

TEST(poly,product_v0L_v02)
{
    ASSERT_EQ( format( product(v0L,v02) ), "0000000000000001000000000000000000000000" );
}

TEST(poly,product_v0L_v0L)
{
    ASSERT_EQ( format( product(v0L,v0L) ), "0000000040000000000000000000000000000000" );
}

TEST(poly,product_v0L_v0M)
{
    ASSERT_EQ( format( product(v0L,v0M) ), "000000007FFFFFFF800000000000000000000000" );
}

TEST(poly,product_v0M_v0)
{
    ASSERT_EQ( format( product(v0M,v0) ), "00000000000000000000000000000000" );
}

TEST(poly,product_v0M_v01)
{
    ASSERT_EQ( format( product(v0M,v01) ), "0000000000000000FFFFFFFF0000000000000000" );
}

TEST(poly,product_v0M_v02)
{
    ASSERT_EQ( format( product(v0M,v02) ), "0000000000000001FFFFFFFE0000000000000000" );
}

TEST(poly,product_v0M_v0L)
{
    ASSERT_EQ( format( product(v0M,v0L) ), "000000007FFFFFFF800000000000000000000000" );
}

TEST(poly,product_v0M_v0M)
{
    ASSERT_EQ( format( product(v0M,v0M) ), "00000000FFFFFFFE000000010000000000000000" );
}

TEST(poly,product_D2836A67C068B195_0986A2ABBA94FA4D)
{
    auto const x = vector { 0xC068B195U, 0xD2836A67U };
    auto const y = vector { 0xBA94FA4DU, 0x0986A2ABU };
    ASSERT_EQ( format( product(x,y) ), "0000000007D5554BB89E1D378484892F090CEBD1" );
}

TEST(poly,product_9A6CD724F1136FB6_D4B89A2A8487D33B)
{
    auto const x = vector { 0xF1136FB6U, 0x9A6CD724U };
    auto const y = vector { 0x8487D33BU, 0xD4B89A2AU };
    ASSERT_EQ( format( product(x,y) ), "0000000080517D64596AF1500E65FEEC5587C0F2" );
}

TEST(poly,product_sum_accumulate_carry)
{
    auto r = vector { 0U, 0xFFFFFFFFU, 0xFFFFFFFFU };
    auto const x = vector { 0x80000000U };
    auto const y = vector { 2U };
    auto const r_ = product_sum_accumulate<unsigned>( span(r), span(x), span(y) );
    ASSERT_EQ( format(r), "000000000000000000000000" );
    ASSERT_EQ( r_, 1U );
}

TEST(poly,product_4_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = vector { generator(), generator(), generator(), generator() };
        auto y = vector { generator(), generator(), generator(), generator() };
        auto r = product<unsigned>( x, y );

        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        mpz_class gr = gx * gy;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "; y = " + format(y) + "\n" +
            "x * y = " + format(r) + "\n"
            "gmp:\n" +
            "x = " + gx.get_str(16) + "; y = " + gy.get_str(16) + "\n"
            "x * y = " + gr.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
    }
}
