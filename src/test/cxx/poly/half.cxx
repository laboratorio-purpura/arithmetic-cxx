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

// Tests.

TEST(poly,half_4_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0; i != 1000; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };

        // compute with purple
        auto q = array<unsigned,4> {};
        auto r = half_assign<unsigned>( q, x, 1 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gq = gx >> 1;
        auto gr = gx % (1 << 1);

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
            mpz_class( format(q), 16 )
        );
        ASSERT_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}

TEST(poly,half_4_31_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0; i != 1000; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator(), generator(), generator() };

        // compute with purple
        auto q = array<unsigned,4> {};
        auto r = half_assign<unsigned>( q, x, 31 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gq = gx >> 31;
        auto gr = gx % (1 << 31);

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
            mpz_class( format(q), 16 )
        );
        ASSERT_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}
