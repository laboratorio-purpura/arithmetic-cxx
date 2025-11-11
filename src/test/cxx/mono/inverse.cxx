// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>
#include <tuple>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.arithmetic.utility;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std::string_literals;

using std::array;
using std::ignore;
using std::span;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST_F(PurpleTest,reciprocal_00000001)
{
    auto y = 0x00000001U;
    auto iy = reciprocal(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST_F(PurpleTest,reciprocal_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = reciprocal(y);
    ASSERT_EQ( format(iy), "00000001" );
}

TEST_P(PurpleRandomTest,reciprocal_random)
{
    constexpr auto B = sizeof(unsigned) * 8;
    using word = unsigned;

    auto generator = GetParam();

    for (auto i = 0uz; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        while (gy == 0u)
            generator->generate(gy,1,B);

        mpz_class giy = mpz_class("FFFFFFFF",16) / gy;

        SCOPED_TRACE("gmp:\n"s +
            "y = " + format(gy) + "\n" +
            "iy = " + format(giy) + "\n"
        );

        // compute with purple

        auto y = array<word,1> {};
        assign(y,gy);

        auto iy = reciprocal( y[0] );

        // compare

        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( giy, to_mpz(iy) );
    }
}

TEST_F(PurpleTest,reciprocal_normalised_80000000)
{
    auto y = 0x80000000U;
    auto iy = reciprocal_normalized(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST_F(PurpleTest,reciprocal_normalised_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = reciprocal_normalized(y);
    ASSERT_EQ( format(iy), "00000001" );
}

TEST_P(PurpleRandomTest,reciprocal_normalized_random)
{
    constexpr auto B = sizeof(unsigned) * 8;
    using word = unsigned;

    auto generator = GetParam();

    for (auto i = 0uz; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator->generate(gy,1,B);
        mpz_setbit(gy.get_mpz_t(),B-1);

        mpz_class giy = ( mpz_class("FFFFFFFFFFFFFFFF",16) / gy ) - mpz_class("100000000",16);

        SCOPED_TRACE("gmp:\n"s +
            "y = " + format(gy) + "\n" +
            "iy = " + format(giy) + "\n"
        );

        // compute with purple

        auto y = array<word,1> {};
        assign(y,gy);

        auto iy = reciprocal_normalized( y[0] );

        SCOPED_TRACE("purple:\n"s +
            "y = " + format(y) + "\n" +
            "iy = " + format(iy) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( giy, to_mpz(iy) );
    }
}
