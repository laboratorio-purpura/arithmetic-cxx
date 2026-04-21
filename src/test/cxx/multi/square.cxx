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

using namespace purple;
using namespace purple::test;
using namespace std::string_literals;

using std::array;
using std::ignore;
using std::span;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST_F(PurpleTest,square_accumulate_regression)
{
    {
        constexpr auto B = 32;
        using word = word<B>;

        // expected
        mpz_class gx { "FFFFFFFFFFFFFFFFFFFFFFFF80000000", 16 };
        mpz_class gr { "FFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000004000000000000000", 16 };
        SCOPED_TRACE( std::string() +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // actual
        auto x = array<word,4> {};
        assign(x,gx);
        auto r = array<word,9> {};
        square_accumulate<word>( r, x );
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare
        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
    {
        constexpr auto B = 32;
        using word = word<B>;

        // expected
        mpz_class gx { "FFFFFFFFFFFFFFFF00007FFF80000000", 16 };
        mpz_class gr { "FFFFFFFFFFFFFFFE0000FFFF00000000FFFF00013FFF80004000000000000000", 16 };
        SCOPED_TRACE( std::string() +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // actual
        auto x = array<word,4> {};
        assign(x,gx);
        auto r = array<word,9> {};
        square_accumulate<word>( r, x );
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare
        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleRandomTest,square_accumulate_32)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 1000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,32,B);

        mpz_class gr = gx * gx;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,32> {};
        assign(x,gx);

        auto r = array<word,64> {};

        square_accumulate<word>( r, x );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}