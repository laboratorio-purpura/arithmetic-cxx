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

TEST(mono,inverse_00000001)
{
    auto y = 0x00000001U;
    auto iy = inverse(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST(mono,inverse_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = inverse(y);
    ASSERT_EQ( format(iy), "00000001" );
}

TEST(mono,inverse_normalised_80000000)
{
    auto y = 0x80000000U;
    auto iy = inverse_normalised(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST(mono,inverse_normalised_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = inverse_normalised(y);
    ASSERT_EQ( format(iy), "00000001" );
}

TEST(mono,inverse_normalised_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto y = generator();
        while (y == 0) y = generator();
        y |= 0x80000000U;
        auto iy = inverse_normalised( y );

        auto gy = mpz_class( format(y), 16 );
        mpz_class giy = ( mpz_class("FFFFFFFFFFFFFFFF",16) / gy ) - mpz_class("100000000",16);

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "y = " + format(y) + "\n" +
            "iy = " + format(iy) + "\n" +
            "gmp:\n" +
            "y = " + gy.get_str(16) + "\n"
            "iy = " + giy.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( giy, mpz_class( format(iy), 16 ) ), 0 );
    }
}
