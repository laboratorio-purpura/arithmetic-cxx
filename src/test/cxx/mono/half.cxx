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

TEST(mono,half_3)
{
    auto [ q, r ] = half(3u,1);
    ASSERT_EQ( q, 1 );
    ASSERT_EQ( r, 1 );
}

TEST(mono,half_7)
{
    auto [ q, r ] = half(7u,1);
    ASSERT_EQ( q, 3 );
    ASSERT_EQ( r, 1 );
}

TEST(mono,half_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = generator();

        // compute with purple
        auto [ q, r ] = half( x, 1 );

        // compute with GMP
        auto gx = mpz_class( format(x), 16 );
        mpz_class gq = gx / 2;
        mpz_class gr = gx % 2;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n"
            "q = " + format(gq) + "\n"
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

TEST(mono,half_2_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = generator();

        // compute with purple
        auto [ q, r ] = half( x, 2 );

        // compute with GMP
        auto gx = mpz_class( format(x), 16 );
        mpz_class gq = gx / 4;
        mpz_class gr = gx % 4;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n"
            "q = " + format(gq) + "\n"
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