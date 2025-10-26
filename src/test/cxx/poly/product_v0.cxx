// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>
#include <vector>

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

TEST(poly,DISABLED_product_v0_D2_D1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        // generate random numbers
        auto x = array { generator(), generator() };
        auto y = array { generator() };

        // compute with purple
        auto v = array { 0u, 0u, 0u, 0u };
        product_v0<unsigned>( v, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gv = gx * gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "v = " + format(v) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "v = " + format(gv) + "\n"
        );
        ASSERT_GMP_EQ(
            gv,
            mpz_class( format(v), 16 )
        );
    }
}
