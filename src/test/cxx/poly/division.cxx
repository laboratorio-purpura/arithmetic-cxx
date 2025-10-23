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

template <typename Integer, size_t Z1, size_t Z2>
auto division_normalized ( array<Integer,Z1> & q, array<Integer,Z2> const & x, Integer y, Integer iy ) -> Integer
{
    auto qq = span<Integer>( q );
    auto xx = span<Integer const>( x );
    return division_normalized( qq, xx, y, iy );
}

TEST(poly,division_normalised_v00_80000000)
{
    auto x = v00;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalized(y);
    auto r = division_normalized<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "0000000000000000" );
    ASSERT_EQ( format(r), "00000000" );
}

TEST(poly,division_normalised_vMM_80000000)
{
    auto x = vMM;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalized(y);
    auto r = division_normalized<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "00000001FFFFFFFF" );
    ASSERT_EQ( format(r), "7FFFFFFF" );
}

TEST(poly,division_normalized_D4_D1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = generator();
        // requires is_normalized(y)
        y |= 0x80000000U;
        // requires is_smaller( x[3], y )
        while ( not_smaller( x[3], y ) ) x[3] = generator();

        // compute with purple
        auto iy = inverse_normalized( y );
        auto q = array { 0u, 0u, 0u, 0u };
        auto r = division_normalized( q, x, y, iy );

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
            "y = " + format(gy) + "\n" +
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
