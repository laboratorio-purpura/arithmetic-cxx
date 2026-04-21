// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>
#include <tuple>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetics;
import purple.arithmetics.utility;
import purple.test;

using namespace purple::arithmetics;
using namespace purple::test;
using namespace std::string_literals;

using std::array;
using std::ignore;
using std::span;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleRandomTest,divide_normal_strict_3_2)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator::generate(gy,2,B);
        mpz_setbit(gy.get_mpz_t(),(2*B)-1);

        mpz_class gx {};
        generator::generate(gx,3,B);
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

        auto x = array<word,3> {};
        assign(x,gx);

        auto y = array<word,2> {};
        assign(y,gy);

        auto iy = reciprocal_normalized<word,2>( y );

        auto [ q, r ] = divide_normal_strict<word,3,2>( x, y, iy );

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