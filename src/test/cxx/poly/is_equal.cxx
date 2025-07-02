// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

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

TEST(poly,is_equal_v0_v0)
{
    auto r = is_equal<unsigned>( span(v0), span(v0) );
    ASSERT_EQ( r, 1 );
}

TEST(poly,is_equal_v0_v1)
{
    auto r = is_equal<unsigned>( span(v0), span(v1) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v0_v01)
{
    auto r = is_equal<unsigned>( span(v0), span(v01) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v1_v0)
{
    auto r = is_equal<unsigned>( span(v1), span(v0) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v1_v1)
{
    auto r = is_equal<unsigned>( span(v1), span(v1) );
    ASSERT_EQ( r, 1 );
}

TEST(poly,is_equal_v1_v01)
{
    auto r = is_equal<unsigned>( span(v1), span(v01) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v1_v10)
{
    auto r = is_equal<unsigned>( span(v1), span(v10) );
    ASSERT_EQ( r, 1 );
}

TEST(poly,is_equal_v00_v1)
{
    auto r = is_equal<unsigned>( span(v00), span(v1) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v01_v1)
{
    auto r = is_equal<unsigned>( span(v01), span(v1) );
    ASSERT_EQ( r, 0 );
}

TEST(poly,is_equal_v10_v1)
{
    auto r = is_equal<unsigned>( span(v10), span(v1) );
    ASSERT_EQ( r, 1 );
}

TEST(poly,is_equal_D4_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = array { generator(), generator(), generator(), generator() };
        auto r = is_equal<unsigned>( span(x), span(y) );

        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gr = gx == gy;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "; y = " + format(y) + "\n" +
            "x < y = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + gx.get_str(16) + "; y = " + gy.get_str(16) + "\n"
            "x < y = " + (gr ? "1" : "0") + "\n"
        );

        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
    }
}
