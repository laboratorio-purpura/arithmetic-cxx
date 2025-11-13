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

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleRandomTest,division_normalized_2_1)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator::generate(gy,1,B);
        mpz_setbit(gy.get_mpz_t(),B-1);

        mpz_class gx {};
        generator::generate(gx,2,B);
        if ((gx >> B) >= gy)
            gx = gx - (gy << B);
        assert( (gx >> B) < gy );

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,2> {};
        assign(x,gx);

        auto y = array<word,1> {};
        assign(y,gy);

        auto iy = reciprocal_normalized( y[0] );

        auto [ q, r ] = division_normalized<word,2>( x, y[0], iy );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}
