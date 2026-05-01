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

TYPED_TEST(PurpleHegelTest,half_accumulate)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(integers<size_t>({.max_value=Bits-1}));
        auto z = size(x);

        // result

        auto r1 = vector<word>(z);
        ignore = half<word>(r1,x,y);

        // accumulate result

        auto r2 = vector<word>(z);
        assign<word>(r2,x);
        ignore = half<word>(r2,r2,y);

        // compare

        if ( ! equal( span(r1), span(r2) ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(r1),format(r2)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,half_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(integers<unsigned>({.max_value=Bits-1}));
        auto z = size(x);

        // compute with purple
        auto r = vector<word>(z);
        half<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto r_ = x_ >> y;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleRandomTest,half_differential)
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

        mpz_class gq = gx >> (Bits-1);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n"
        );

        // compute with purple

        auto x = vector<word>(64);
        assign<word>(x,gx);

        auto q = vector<word>(64);

        ignore = half<word>( q, x, (Bits-1) );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
    }
}

TYPED_TEST(PurpleHegelTest,half_short)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(integers<size_t>({.max_value=Bits-1}));
        // full size
        auto fz = size(x);
        // short size
        auto sz = tc.draw(integers<size_t>({.max_value=fz-1}));

        // compute full result

        auto fr = vector<word>(fz);
        auto _ = half<word>(fr,x,y);

        // compute short result

        auto sr = vector<word>(sz);
        auto _ = half<word>(sr,x,y);

        // compare

        if ( ! equal( span(fr).subspan(0,sz), span(sr).subspan(0,sz) ) )
            throw runtime_error(fmt::format("fr = {}, sr = {}",format(fr),format(sr)));
    },
    { .test_cases = hegel_cases });
}
