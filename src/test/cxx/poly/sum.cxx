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

TEST_F(PurpleTest,sum_assign_N_1_capacity)
{
    array<unsigned,1> r1 {};
    array<unsigned,2> r2 {};
    array<unsigned,3> r3 {};

    // tests required capacity when operands are not compact

    auto carry = sum_assign<unsigned>( r1, array { 0u, 0u }, 1u );
    ASSERT_EQ( format(r1), "00000001" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r1, array { 0xFFFFFFFEu, 0u }, 1u );
    ASSERT_EQ( format(r1), "FFFFFFFF" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r1, array { 0xFFFFFFFFu, 0u }, 1u );
    ASSERT_EQ( format(r1), "00000000" );
    ASSERT_EQ( carry, 1 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFFu, 0u }, 1u );
    ASSERT_EQ( format(r2), "0000000100000000" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFEu, 0xFFFFFFFFu }, 1u );
    ASSERT_EQ( format(r2), "FFFFFFFFFFFFFFFF" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFFu, 0xFFFFFFFFu }, 1u );
    ASSERT_EQ( format(r2), "00000000" );
    ASSERT_EQ( carry, 1 );

    carry = sum_assign<unsigned>( r3, array { 0xFFFFFFFFu, 0xFFFFFFFFu }, 1u );
    ASSERT_EQ( format(r3), "000000010000000000000000" );
    ASSERT_EQ( carry, 0 );
}

TEST_F(PurpleTest,sum_assign_64_1_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64> {};
        generate(x);

        auto y = generator();

        // purposeful excess capacity with garbage
        auto r = array<unsigned,66> {};
        generate(r);

        // compute with purple
        ignore = sum_assign<unsigned>( r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gr = gx + y;

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

TEST_F(PurpleTest,sum_assign_N_capacity)
{
    array<unsigned,1> r1 {};
    array<unsigned,2> r2 {};
    array<unsigned,3> r3 {};

    // tests required capacity when operands are not compact

    auto carry = sum_assign<unsigned>( r1, array { 0u, 0u }, array { 1u, 0u } );
    ASSERT_EQ( format(r1), "00000001" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r1, array { 0xFFFFFFFEu, 0u }, array { 1u, 0u } );
    ASSERT_EQ( format(r1), "FFFFFFFF" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r1, array { 0xFFFFFFFFu, 0u }, array { 1u, 0u } );
    ASSERT_EQ( format(r1), "00000000" );
    ASSERT_EQ( carry, 1 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFFu, 0u }, array { 1u, 0u } );
    ASSERT_EQ( format(r2), "0000000100000000" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFEu, 0xFFFFFFFFu }, array { 1u, 0u } );
    ASSERT_EQ( format(r2), "FFFFFFFFFFFFFFFF" );
    ASSERT_EQ( carry, 0 );

    carry = sum_assign<unsigned>( r2, array { 0xFFFFFFFFu, 0xFFFFFFFFu }, array { 1u, 0u } );
    ASSERT_EQ( format(r2), "00000000" );
    ASSERT_EQ( carry, 1 );

    carry = sum_assign<unsigned>( r3, array { 0xFFFFFFFFu, 0xFFFFFFFFu }, array { 1u, 0u } );
    ASSERT_EQ( format(r3), "000000010000000000000000" );
    ASSERT_EQ( carry, 0 );
}

TEST_F(PurpleTest,sum_assign_32_32_random)
{
    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,32> {};
        generate(x);

        auto y = array<unsigned,32> {};
        generate(y);

        // purposeful excess capacity with garbage
        auto r = array<unsigned,34> {};
        generate(r);

        // compute with purple
        ignore = sum_assign<unsigned>( r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gr = gx + gy;

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
