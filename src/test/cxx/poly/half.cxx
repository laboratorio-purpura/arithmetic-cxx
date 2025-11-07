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
using std::ignore;
using std::span;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Tests.

TEST_F(PurpleTest,half_assign_64_1_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64> {};
        generate(x);

        // purposeful excess capacity with garbage
        auto r = array<unsigned,66> {};
        generate(r);

        // compute with purple
        ignore = half_assign<unsigned>( r, x, 1 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx >> 1;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}

TEST_F(PurpleTest,half_assign_64_31_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64> {};
        generate(x);

        // purposeful excess capacity with garbage
        auto r = array<unsigned,66> {};
        generate(r);

        // compute with purple
        ignore = half_assign<unsigned>( r, x, 31 );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx >> 31;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}
