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
using std::random_device;
using std::span;

using random_engine = std::linear_congruential_engine<unsigned,48271ul,0ul,2147483647ul>;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Tests.

TEST_F(PurpleTest,previous_assign_capacity)
{
    auto r2 = array<unsigned,2> {};
    unsigned b {};

    b = previous_assign<unsigned>( r2, array { 0u } );
    ASSERT_EQ( format(r2), "FFFFFFFFFFFFFFFF" );
    ASSERT_EQ( b, 1 );

    b = previous_assign<unsigned>( r2, array { 0u, 0u } );
    ASSERT_EQ( format(r2), "FFFFFFFFFFFFFFFF" );
    ASSERT_EQ( b, 1 );
}

TEST_F(PurpleTest,previous_assign_32_nonzero_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,32> {};
        while ( is_zero<unsigned>( x ) )
            generate(x);

        // purposeful excess capacity with garbage
        auto r = array<unsigned,34> {};
        generate(r);

        // compute with purple
        ignore = previous_assign<unsigned>( r, x );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = --gx;

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