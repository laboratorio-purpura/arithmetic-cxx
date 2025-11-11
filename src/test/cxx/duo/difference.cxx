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
using std::swap;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST_P(PurpleRandomTest,difference_assign_2_2)
{
    constexpr auto B = sizeof(unsigned) * 8;
    using word = unsigned;

    auto generator = GetParam();

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gx {};
        generator->generate(gx,2,B);

        mpz_class gy {};
        generator->generate(gy,2,B);

        if (gx < gy)
            swap(gx,gy);

        mpz_class gr = gx - gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,2> {};
        assign(x,gx);

        auto y = array<word,2> {};
        assign(y,gy);

        // purposeful excess capacity with garbage
        auto r = array<word,4> {};
        generator->generate(r);

        ignore = difference_assign<word>( r, x, y );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}
