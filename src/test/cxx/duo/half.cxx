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

template <typename Word, size_t Degree>
requires ( Degree == 2uz )
auto half_accumulate ( array<Word,Degree> & x, size_t N ) noexcept -> Word
{
    auto xx = span<Word,Degree>( x );
    return half_assign( xx, N );
}

TEST(duo,half_accumulate_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator() };

        // compute with purple
        auto q = x;
        auto r = half_accumulate( q, 1 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gq = gx / 2;
        auto gr = gx % 2;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format( q ), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format( r ), 16 )
        );
    }
}

TEST(duo,half_accumulate_2_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator() };

        // compute with purple
        auto q = x;
        auto r = half_accumulate( q, 2 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gq = gx / 4;
        auto gr = gx % 4;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format( q ), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format( r ), 16 )
        );
    }
}
