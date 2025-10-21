// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.arithmetic.alternative;
import purple.test;

using namespace purple;
using namespace purple::test;

using std::array;
using std::ignore;
using std::random_device;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;
using std::span;

/// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

/// Values.

auto x_4291E1FD61304D0E65C8E202031145A2 = array { 0x031145A2u, 0x65C8E202u, 0x61304D0Eu, 0x4291E1FDu };
auto y_F15E9C694199994951736A63 = array { 0x51736A63u, 0x41999949u, 0xF15E9C69u };

/// division_v1_restricted_accumulate

template <typename T, size_t XZ, size_t YZ>
void division_v1_restricted_accumulate_test ( array<T,XZ> const & x, array<T,YZ> const & y )
{
    // compute with purple
    auto q = T(0);
    auto r = x;
    auto iy = inverse_normalized( y[YZ-1] );
    division_v1_restricted_accumulate<unsigned>( q, r, y, iy );

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

TEST(poly,division_v1_restricted_accumulate_4291E1FD61304D0E65C8E202031145A2_F15E9C694199994951736A63)
{
    auto x = x_4291E1FD61304D0E65C8E202031145A2;
    auto y = y_F15E9C694199994951736A63;
    division_v1_restricted_accumulate_test(x,y);
}

TEST(poly,division_v1_restricted_accumulate_D4_D3_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = array { generator(), generator(), generator() };

        // requires is_normalized(y)
        y[2] |= 0x80000000;

        // requires is_smaller( x[3], y[2] )
        while ( not_smaller( x[3], y[2] ) ) ignore = difference_accumulate( x[3], y[2] );

        division_v1_restricted_accumulate_test( x,  y );
    }
}

/// division_v1_normalized_accumulate

template <typename T, size_t ZX, size_t ZY>
void division_v1_normalized_accumulate_test ( array<T,ZX> const & x, array<T,ZY> const & y )
{
    // compute with purple
    auto q = array<T,ZY> {};
    auto r = x;
    division_v1_normalized_accumulate<unsigned>( q, r, y );

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

TEST(poly,division_v1_normalized_accumulate_4291E1FD61304D0E65C8E202031145A2_F15E9C694199994951736A63)
{
    auto x = x_4291E1FD61304D0E65C8E202031145A2;
    auto y = y_F15E9C694199994951736A63;
    division_v1_normalized_accumulate_test(x,y);
}

TEST(poly,division_v1_normalized_accumulate_D4_D3_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = array { generator(), generator(), generator() };

        // requires is_normalized(y)
        y[2] |= 0x80000000;

        // requires is_smaller( x[3], y[2] )
        while ( not_smaller( x[3], y[2] ) ) ignore = difference_accumulate( x[3], y[2] );

        division_v1_normalized_accumulate_test( x, y );
    }
}

/// division_v1_accumulate

template <typename T, size_t XZ, size_t YZ>
void division_v1_test ( array<T,XZ> const & x, array<T,YZ> const & y )
{
    // compute with purple
    auto q = array<T,XZ> {};
    auto r = array<T,XZ+1> {};
    division_v1<unsigned>( q, r, x, y );

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

TEST(poly,division_v1_accumulate_D4_D2_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = array { generator(), generator() };

        // requires not_zero(y)
        while ( is_zero<unsigned>( y ) ) y = array { generator(), generator() };

        // TODO: lift this restriction
        // requires is_smaller( x[3], y[1] )
        // while ( not_smaller( x[3], y[1] ) ) ignore = difference_accumulate( x[3], y[1] );

        division_v1_test( x, y );
    }
}