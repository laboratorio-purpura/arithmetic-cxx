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

template <typename Word, size_t TriDegree, size_t BiDegree>
requires  ( TriDegree == 3uz ) && ( BiDegree == 2uz )
auto division_normalized ( array<Word,TriDegree> const & x, array<Word,BiDegree> const & y, Word iy ) noexcept
{
    auto xx = span<Word const,TriDegree>( x );
    auto yy = span<Word const,BiDegree>( y );
    return division_normalized<Word,TriDegree,BiDegree>( xx, yy, iy );
}

template <typename Word, size_t Degree>
requires ( Degree == 2uz )
auto inverse_normalized ( array<Word,Degree> const & y ) noexcept
{
    auto yy = span<Word const,Degree>( y );
    return reciprocal_normalized<Word,Degree>( yy );
}

template <typename Word, size_t Degree>
requires ( Degree == 2uz )
auto not_smaller ( array<Word,Degree> const & x, array<Word,Degree> const & y ) noexcept
{
    auto xx = span<Word const,Degree>( x );
    auto yy = span<Word const,Degree>( y );
    return not_smaller<Word,Degree>( xx, yy );
}

TEST(tri,division_normalized_3_2_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator() };
        auto y = array { generator(), generator() };
        // requires is_normalized(y)
        y[1] |= 0x80000000U;
        // requires is_smaller( { x[1], x[2] }, y )
        while ( not_smaller( array { x[1], x[2] }, y ) ) x[2] = random();

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
