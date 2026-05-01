// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

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

TYPED_TEST(PurpleHegelTest,sum_N_1_accumulate)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(words<Bits>());
        auto z = size(x);

        // result

        auto r1 = vector<word>(z);
        ignore = sum<word>(r1,x,y);

        // accumulate result

        auto r2 = vector<word>(z);
        assign<word>(r2,x);
        ignore = sum<word>(r2,r2,y);

        // compare

        if ( ! equal( r1, r2 ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(r1),format(r2)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,sum_N_1_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(words<Bits>());

        // compute with purple
        auto z = size(x);
        auto r = vector<word>(z+1);
        r[z] = sum<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ + y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleRandomTest,sum_N_1_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using generator = tuple_element<1,TypeParam>::type;
    using word = word<Bits>;

    for (auto i = 0; i != random_cases; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,Bits);

        mpz_class gy {};
        generator::generate(gy,1,Bits);

        mpz_class gr = gx + gy;

        SCOPED_TRACE("gmp:\n"s +
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

        r[64] = sum<word>( r, x, y[0] );

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

TYPED_TEST(PurpleHegelTest,sum_N_1_short_result)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(words<Bits>());
        // full size
        auto fz = size(x);
        // short size
        auto sz = tc.draw(integers<size_t>({.max_value=fz-1}));

        // full result

        auto fr = vector<word>(fz);
        auto _ = sum<word>(fr,x,y);

        // short result

        auto sr = vector<word>(sz);
        auto _ = sum<word>(sr,x,y);

        // compare

        if ( ! equal( span(fr).subspan(0,sz), span(sr).subspan(0,sz) ) )
            throw runtime_error(fmt::format("fr = {}, sr = {}",format(fr),format(sr)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,sum_accumulate)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto z = max( size(x), size(y) );

        // result

        auto r1 = vector<word>(z);
        ignore = sum<word>(r1,x,y);

        // accumulate result

        auto r2 = vector<word>(z);
        assign<word>(r2,x);
        ignore = sum<word>(r2,r2,y);

        // compare

        if ( ! equal( r1, r2 ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(r1),format(r2)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,sum_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));

        // compute with purple
        auto z = max(size(x),size(y));
        auto r = vector<word>(z+1);
        r[z] = sum<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ + y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleRandomTest,sum_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using generator = tuple_element<1,TypeParam>::type;
    using word = word<Bits>;

    for (auto i = 0; i != random_cases; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,32,Bits);

        mpz_class gy {};
        generator::generate(gy,32,Bits);

        mpz_class gr = gx + gy;

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

        auto r = vector<word>(33);

        r[32] = sum<word>( r, x, y );

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

TYPED_TEST(PurpleHegelTest,sum_short_result)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        // full size
        auto fz = max(size(x),size(y));
        // short size
        auto sz = tc.draw(integers<size_t>({.max_value=fz-1}));

        // full result

        auto fr = vector<word>(fz);
        ignore = sum<word>(fr,x,y);

        // short result

        auto sr = vector<word>(sz);
        ignore = sum<word>(sr,x,y);

        // compare

        if ( ! equal( span(fr).subspan(0,sz), span(sr).subspan(0,sz) ) )
            throw runtime_error(fmt::format("fr = {}, sr = {}",format(fr),format(sr)));
    },
    { .test_cases = hegel_cases });
}