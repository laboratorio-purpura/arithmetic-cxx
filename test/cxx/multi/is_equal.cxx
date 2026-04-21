// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>
#include <tuple>

#include <fmt/format.h>

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

TYPED_TEST(PurpleRandomTest,is_equal_64)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        auto gr = (gx == gx);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "r = " + fmt::format("{}",gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto r = are_equal<word>( x, x );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "r = " + fmt::format("{}",r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_EQ( gr, r );
    }
}

TYPED_TEST(PurpleRandomTest,is_equal_64_64)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        mpz_class gy {};
        generator::generate(gy,64,B);

        auto gr = (gx == gy);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + fmt::format("{}",gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto y = array<word,64> {};
        assign(y,gy);

        auto r = are_equal<word>( x, y );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "r = " + fmt::format("{}",r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_EQ( gr, r );
    }
}