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

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST(duo,reciprocal_normalised_9C8739F05AA157B2)
{
    auto y = array { 0x5AA157B2U, 0x9C8739F0U };
    auto iy = reciprocal_normalized( y );
    ASSERT_EQ( format(iy), "A2AF5356" );
}

TEST(duo,reciprocal_normalised_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 100; ++i)
    {
        auto y = array { generator(), generator() };
        // requires is_normalized(y)
        y[1] |= 0x80000000U;

        // compute with purple
        auto iy = reciprocal_normalized( y );

        // compute with GMP
        auto gy = mpz_class( format(y), 16 );
        mpz_class giy = ( mpz_class("FFFFFFFFFFFFFFFFFFFFFFFF",16) / gy ) - mpz_class("100000000",16);

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "y = " + format(y) + "\n" +
            "iy = " + format(iy) + "\n" +
            "gmp:\n" +
            "y = " + format(gy) + "\n"
            "iy = " + format(giy) + "\n"
        );
        ASSERT_GMP_EQ(
            giy,
            mpz_class( format(iy), 16 )
        );
    }
}
