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

TEST_F(PurpleTest,difference_assign_2_2)
{
    auto r = array<unsigned,32> {};
    auto b = 0u;

    b = difference_assign<unsigned,2>( r,
        array { 0x5F327ECDu, 0x4253ECBFu },
        array { 0x2EBDE466u, 0x0FCDC1D5u }
    );
    ASSERT_EQ( format(r), "32862AEA30749A67" );
    ASSERT_EQ( b , 0 );
}

TEST_F(PurpleTest,difference_assign_2_2_borrowless_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,2> {};
        auto y = array<unsigned,2> {};

        while ( not_greater<unsigned,2>( x, y ) ) {
            generate(x);
            generate(y);
        }

        // purposeful excess capacity with garbage
        auto r = array<unsigned,4> {};
        generate(r);

        // compute with purple
        ignore = difference_assign<unsigned,2>( r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gr = gx - gy;

        // compare
        SCOPED_TRACE( std::string() +
            "i = " + format(i) + "\n" +
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
