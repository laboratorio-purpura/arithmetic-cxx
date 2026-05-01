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

TYPED_TEST(PurpleHegelTest,twice_accumulate)
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
        ignore = twice<word>(r1,x,y);

        // accumulate result

        auto r2 = vector<word>(z);
        assign<word>(r2,x);
        ignore = twice<word>(r2,r2,y);

        // compare

        if ( ! equal( span(r1), span(r2) ) )
            throw runtime_error(fmt::format("r1 = {}, r2 = {}",format(r1),format(r2)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,twice_differential)
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
        auto r = vector<word>(z+1);
        r[z] = twice<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto r_ = x_ << y;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleRandomTest,twice_differential)
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

        mpz_class gr = gx << (Bits-1);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "r = " + format(gr) + "\n"
        );        // compute with purple

        auto x = vector<word>(64);
        assign<word>(x,gx);

        auto r = vector<word>(65);

        r[64] = twice<word>( r, x, (Bits-1) );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleHegelTest,twice_short_result)
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

        // full result

        auto fr = vector<word>(fz);
        auto _ = twice<word>(fr,x,y);

        // short result

        auto sr = vector<word>(sz);
        auto _ = twice<word>(sr,x,y);

        // compare

        if ( ! equal( span(fr).subspan(0,sz), span(sr) ) )
            throw runtime_error(fmt::format("fr = {}, sr = {}",format(fr),format(sr)));
    },
    { .test_cases = hegel_cases });
}
