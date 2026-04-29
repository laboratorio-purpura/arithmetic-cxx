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
using std::ignore;
using std::runtime_error;
using std::span;
using std::tuple_element;
using std::vector;
using namespace std::ranges;
using namespace std::literals;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleHegelTest,product_N_1_differential_gmp)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(words<Bits>());

        // compute with purple
        auto z = size(x);
        auto r = vector<word>(z+1);
        r[z] = product<word>(r,x,y);

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

TYPED_TEST(PurpleRandomTest,product_64_1_differential_gmp)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using generator = tuple_element<1,TypeParam>::type;
    using word = word<Bits>;

    for (auto i = 0; i != 100000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,Bits);

        mpz_class gy {};
        generator::generate(gy,1,Bits);

        mpz_class gr = gx * gy;

        SCOPED_TRACE( std::string() +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = vector<word>(64);
        assign<word>(x,gx);

        auto y = vector<word>(1);
        assign<word>(y,gy);

        auto r = vector<word>(65);

        r[64] = product<word>( r, x, y[0] );

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

TYPED_TEST(PurpleHegelTest,product_N_1_size)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(words<Bits>());
        // full result size
        auto fz = size(x);
        // variable result size
        auto vz = tc.draw(integers<size_t>({.max_value=128}));

        // compute full-sized result

        auto fr = vector<word>(fz);
        auto _ = product<word>(fr,x,y);

        // compute variable-sized result

        auto vr = vector<word>(vz);
        auto _ = product<word>(vr,x,y);

        // compare

        auto z = min(fz, vz);
        if ( ! equal( span(fr).subspan(0,z), span(vr).subspan(0,z) ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(fr),format(vr)));
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleHegelTest,product_differential_gmp)
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
        product<word>(r,x,y);

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

TYPED_TEST(PurpleRandomTest,product_32_32_differential_gmp)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using generator = tuple_element<1,TypeParam>::type;
    using word = word<Bits>;

    for (auto i = 0; i != 100000; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,32,Bits);

        mpz_class gy {};
        generator::generate(gy,32,Bits);

        mpz_class gr = gx * gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = vector<word>(32);
        assign<word>(x,gx);

        auto y = vector<word>(32);
        assign<word>(y,gy);

        auto r = vector<word>(64);

        product<word>( r, x, y );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "z = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleHegelTest,product_size)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        // full result size
        auto fz = size(x) + size(y);
        // variable result size
        auto vz = tc.draw(integers<size_t>({.max_value=128}));

        // compute full-sized result

        auto fr = vector<word>(fz);
        product<word>(fr,x,y);

        // compute variable-sized result

        auto vr = vector<word>(vz);
        product<word>(vr,x,y);

        // compare

        auto z = min(fz, vz);
        if ( ! equal( span(fr).subspan(0,z), span(vr).subspan(0,z) ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(fr),format(vr)));
    },
    { .test_cases = 1000 });
}
