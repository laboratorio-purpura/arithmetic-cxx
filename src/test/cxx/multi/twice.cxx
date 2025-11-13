// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

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

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleRandomTest,twice_assign_64)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        mpz_class gr = gx << (B-1);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        // purposeful excess capacity with garbage
        auto r = array<word,66> {};
        generator::generate(r);

        ignore = twice_assign<word>( r, x, (B-1) );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}