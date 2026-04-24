// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

#include <gmpxx.h>
#include <gtest/gtest.h>
#include <hegel/hegel.h>

#include <fmt/format.h>
#include <gmpxx.h>
#include <gtest/gtest.h>
#include <hegel/hegel.h>

import purple.arithmetics;
import purple.arithmetics.utility;
import purple.test;

using namespace hegel::generators;
using namespace purple::arithmetics;
using namespace purple::test;
using namespace std;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleHegelTest,multiply_N_1_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(words<Bits>());

        // compute with purple
        auto z = size(x);
        auto r = vector<word>(z+1);
        r[z] = multiply<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ * y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleRandomTest,multiply_64_1_differential_gmp)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 100000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        mpz_class gy {};
        generator::generate(gy,1,B);

        mpz_class gr = gx * gy;

        SCOPED_TRACE( std::string() +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto y = array<word,1> {};
        assign(y,gy);

        auto r = array<word,65> {};

        r[64] = multiply<word>( r, x, y[0] );

        SCOPED_TRACE( std::string() +
            "purple:\n" +
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

TYPED_TEST(PurpleHegelTest,multiply_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));

        // compute with purple
        auto z = size(x) + size(y);
        auto r = vector<word>(z);
        multiply2<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ * y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error(fmt::format("r = {}, r_ = {}",format(r),format(r_)));
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleRandomTest,multiply_32_32_differential_gmp)
{
    constexpr auto B = tuple_element<0,TypeParam>::type::value;
    constexpr auto N = 2uz;
    using generator = tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 100000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,N,B);

        mpz_class gy {};
        generator::generate(gy,N,B);

        mpz_class gz {};
        generator::generate(gy,N,B);

        mpz_class gr = ( gx * gy ) + gz;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,N> {};
        assign(x,gx);

        auto y = array<word,N> {};
        assign(y,gy);

        auto r = array<word,N*2> {};
        assign(r,gz);

        multiply2<word>( r, x, y );

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
