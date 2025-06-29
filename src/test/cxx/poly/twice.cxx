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

TEST(poly,twice_v0)
{
    ASSERT_EQ( format( twice(v0) ), "0000000000000000" );
}

TEST(poly,twice_v1)
{
    ASSERT_EQ( format( twice(v1) ), "0000000000000002" );
}

TEST(poly,twice_v2)
{
    ASSERT_EQ( format( twice(v2) ), "0000000000000004" );
}

TEST(poly,twice_vL)
{
    ASSERT_EQ( format( twice(vL) ), "0000000100000000" );
}

TEST(poly,twice_vM)
{
    ASSERT_EQ( format( twice(vM) ), "00000001FFFFFFFE" );
}

TEST(poly,twice_v01)
{
    ASSERT_EQ( format( twice(v01) ), "000000000000000200000000" );
}

TEST(poly,twice_v02)
{
    ASSERT_EQ( format( twice(v02) ), "000000000000000400000000" );
}

TEST(poly,twice_v0L)
{
    ASSERT_EQ( format( twice(v0L) ), "000000010000000000000000" );
}

TEST(poly,twice_v0M)
{
    ASSERT_EQ( format( twice(v0M) ), "00000001FFFFFFFE00000000" );
}

TEST(poly,twice_4_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = vector { generator(), generator(), generator(), generator() };
        auto r = twice<unsigned>( x );

        auto gx = mpz_class( format(x), 16 );
        mpz_class gr = gx * 2;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "x * 2 = " + format(r) + "\n"
            "gmp:\n" +
            "x = " + gx.get_str(16) + "\n"
            "x * 2 = " + gr.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
    }
}
