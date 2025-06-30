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

TEST(tri,ratio_random)
{
    random_device random;
    random_integer generator { random() };

    auto gx = mpz_class();
    auto gy = mpz_class();

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = array { generator(), generator(), generator() };
        auto y = array { generator(), generator() };
        while (y[1] == 0) y[1] = generator();
        y[1] |= 0x80000000U;
        while ( not_smaller( to_cspan<2>(x,1), to_cspan<2>(y) ) )
            x[2] = random();
        auto iy = inverse_normalised( y );
        auto [ q, r ] = ratio_normalised( x, y, iy );

        gx.set_str( format(x), 16 );
        gy.set_str( format(y), 16 );
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
