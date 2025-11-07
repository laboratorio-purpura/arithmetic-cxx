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
using std::ignore;
using std::span;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Tests.

TEST_F(PurpleTest,product_assign_64_1_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64> {};
        generate(x);

        auto y = generate();

        // purposeful excess capacity with garbage
        auto r = array<unsigned,66> {};
        generate(r);

        // compute with purple
        ignore = product_assign<unsigned>( r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx * y;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(y) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}

TEST_F(PurpleTest,product_accumulate_32_32_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,32> {};
        generate(x);

        auto y = array<unsigned,32> {};
        generate(y);

        auto r = array<unsigned,65> {};

        // compute with purple
        ignore = product_accumulate<unsigned>( r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gr = gx * gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}