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

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Type conversion from array.

template <typename Integer, size_t Degree>
requires ( Degree == 2uz )
auto twice_accumulate ( array<Integer,Degree> & x, size_t N ) noexcept -> Integer
{
    auto xx = span<Integer,Degree>( x );
    return twice_assign( xx, N );
}

TEST(duo,twice_accumulate_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator() };

        // compute with
        auto r = x;
        auto rh = twice_accumulate( r, 1 );

        auto gx = mpz_class( format(x), 16 );
        auto gr = gx * 2;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format( array { r[0], r[1], rh } ) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format( array { r[0], r[1], rh } ), 16 )
        );
    }
}