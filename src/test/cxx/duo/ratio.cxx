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
using std::span;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;

TEST(duo,ratio_v00_80000000)
{
    auto x = v00;
    auto y = 0x80000000U;
    auto [ q, r ] = ratio<unsigned>( span<unsigned,2>(x), y );
    ASSERT_EQ( format(q), "0000000000000000" );
    ASSERT_EQ( format(r), "00000000" );
}

TEST(duo,ratio_vMM_80000000)
{
    auto x = vMM;
    auto y = 0x80000000U;
    auto [ q, r ] = ratio<unsigned>( span<unsigned,2>(x), y );
    ASSERT_EQ( format(q), "00000001FFFFFFFF" );
    ASSERT_EQ( format(r), "7FFFFFFF" );
}

TEST(duo,ratio_0F703AE14EC018B1_0AEE366D)
{
    auto x = array { 0x4EC018B1U, 0x0F703AE1U };
    auto y = 0x0AEE366DU;
    auto [ q, r ] = ratio<unsigned>( x, y );
    ASSERT_EQ( format(q), "000000016993DADE" );
    ASSERT_EQ( format(r), "043D142B" );
}

TEST(duo,ratio_random)
{
    random_device random;
    random_integer generator { random() };

    auto x = array<unsigned,2> {};
    auto y = generator();

    auto gx = mpz_class();
    auto gy = mpz_class();

    for (auto i = 0uz; i != 10; ++i)
    {
        x = { generator(), generator() };
        y = generator();
        while (y == 0) y = generator();

        auto [ q, r ] = ratio<unsigned>( x, y );

        gx.set_str( format(x), 16 );
        gy.set_str( format(y), 16 );

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "; y = " + format(y) + "\n" +
            "q = " + format(q) + "; r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + gx.get_str(16) + "; y = " + gy.get_str(16) + "\n"
            "q = " + gq.get_str(16) + "; r = " + gr.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( gq, mpz_class( format(q), 16 ) ), 0 );
        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
    }
}
