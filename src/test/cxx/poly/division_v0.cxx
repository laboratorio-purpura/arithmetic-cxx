// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.arithmetic.alternative;
import purple.test;

using namespace purple;
using namespace purple::test;

using std::array;
using std::random_device;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;
using std::span;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST(poly,DISABLED_division_v0_D2_D1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        // generate random numbers
        auto x = array { generator(), 1u };
        auto y = array { generator() };
        // requires not_zero(y)
        while ( is_zero<unsigned>(y) ) y = array { generator() };

        // compute with purple
        auto q = array { 0u, 0u };
        auto r = array { 0u, 0u };
        division_v0<unsigned>( q, r, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gq = gx / gy;
        auto gr = gx % gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format(q), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}
