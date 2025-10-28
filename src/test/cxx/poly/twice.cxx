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

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Type conversion for arrays.

template <typename Word, size_t Z>
auto twice_accumulate ( array<Word,Z> & x, size_t N, Word carry = Word(0) ) noexcept -> Word
{
    auto xx = span<Word>( x );
    return twice_assign( xx, N, carry );
}

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

TEST(poly,twice_D4_N1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };

        // compute with purple
        auto r = x;
        auto rh = twice_accumulate( r, 1 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx * 2;

        // compute
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format( array { r[0], r[1], r[2], r[3], rh } ) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format( array { r[0], r[1], r[2], r[3], rh } ), 16 )
        );
    }
}

TEST(poly,twice_D4_N2_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };

        // compute with purple
        auto r = x;
        auto rh = twice_accumulate( r, 2 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx * 4;

        // compute
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format( array { r[0], r[1], r[2], r[3], rh } ) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format( array { r[0], r[1], r[2], r[3], rh } ), 16 )
        );
    }
}