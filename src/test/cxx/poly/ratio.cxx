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

TEST(poly,division_normalised_v00_80000000)
{
    auto x = v00;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalized(y);
    auto r = division_normalized<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "0000000000000000" );
    ASSERT_EQ( format(r), "00000000" );
}

TEST(poly,division_normalised_vMM_80000000)
{
    auto x = vMM;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalized(y);
    auto r = division_normalized<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "00000001FFFFFFFF" );
    ASSERT_EQ( format(r), "7FFFFFFF" );
}

TEST(poly,division_4_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = array { generator(), generator(), generator(), generator() };
        auto y = generator();
        while (y == 0) y = generator();
        y |= 0x80000000U;
        auto q = array<unsigned,4>();
        auto r = division_normalized<unsigned>( q, x, y );

        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "; y = " + format(y) + "\n" +
            "x / y = " + format(q) + "; x % y = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + gx.get_str(16) + "; y = " + gy.get_str(16) + "\n"
            "x / y = " + gq.get_str(16) + "; x % y = " + gr.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( gq, mpz_class( format(q), 16 ) ), 0 );
        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
    }
}
