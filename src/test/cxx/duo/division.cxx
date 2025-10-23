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

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Type conversion from array.

template <typename Integer, size_t Degree>
requires ( Degree == 2uz )
auto division_normalized ( array<Integer,Degree> const & x, Integer y, Integer iy ) noexcept
{
    auto xx = span<Integer const,Degree>( x );
    return division_normalized<Integer,Degree>( xx, y, iy );
}

TEST(duo,division_normalized_2_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        auto x = array { generator(), generator() };
        auto y = generator();
        // requires is_normalized( y )
        y |= 0x80000000;
        // requires is_smaller( x1[1], y )
        while ( not_smaller( x[1], y ) ) x[1] = generator();

        // compute with purple
        auto iy = inverse_normalized( y );
        auto [ q, r ] = division_normalized( x, y, iy );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gq = gx / gy;
        auto gr = gx % gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format(q), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}

TEST(duo,division_v00_80000000)
{
    auto x = v00;
    auto y = 0x80000000U;
    auto [ q, r ] = division( to_cspan<2>(x), y );
    ASSERT_EQ( format(q), "0000000000000000" );
    ASSERT_EQ( format(r), "00000000" );
}

TEST(duo,division_vMM_80000000)
{
    auto x = vMM;
    auto y = 0x80000000U;
    auto [ q, r ] = division( to_cspan<2>(x), y );
    ASSERT_EQ( format(q), "00000001FFFFFFFF" );
    ASSERT_EQ( format(r), "7FFFFFFF" );
}

TEST(duo,division_0F703AE14EC018B1_0AEE366D)
{
    auto x = array { 0x4EC018B1U, 0x0F703AE1U };
    auto y = 0x0AEE366DU;
    auto [ q, r ] = division<unsigned,2>( x, y );
    ASSERT_EQ( format(q), "000000016993DADE" );
    ASSERT_EQ( format(r), "043D142B" );
}

TEST(duo,division_2_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        auto x = array { generator(), generator() };
        auto y = generator();
        // requires not_zero(y)
        while ( is_zero(y) ) y = generator();

        // compute with purple
        auto [ q, r ] = division<unsigned,2>( x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gq = gx / gy;
        auto gr = gx % gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format(q), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}