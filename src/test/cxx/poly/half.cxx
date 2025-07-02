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
using std::random_device;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;
using std::span;
using std::vector;

TEST(poly,half_4_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 10; ++i)
    {
        auto x = vector { generator(), generator(), generator(), generator() };

        auto r = x;
        auto r_ = half_accumulate<unsigned>( span(r), 1 );

        auto gx = mpz_class( format(x), 16 );
        mpz_class gr = gx / 2;
        mpz_class gr_ = gx % 2;

        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "x * 2 = " + format(r) + "\n" +
            "x % 2 = " + format(r_) + "\n" +
            "gmp:\n" +
            "x = " + gx.get_str(16) + "\n" +
            "x * 2 = " + gr.get_str(16) + "\n" +
            "x % 2 = " + gr_.get_str(16) + "\n"
        );

        ASSERT_EQ( cmp( gr, mpz_class( format(r), 16 ) ), 0 );
        ASSERT_EQ( cmp( gr_, mpz_class( format(r_), 16 ) ), 0 );
    }
}
