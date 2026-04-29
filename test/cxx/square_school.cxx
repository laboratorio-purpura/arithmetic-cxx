// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <array>
#include <span>
#include <tuple>

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
using std::array;
using std::ignore;
using std::runtime_error;
using std::span;
using std::tuple_element;
using std::vector;
using namespace std::ranges;
using namespace std::literals;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleRandomTest,square_school_32_differential_gmp)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i < random_cases; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

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

        square_school<word>( r, x );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleHegelTest,square_school_differential_gmp)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));

        // compute with purple

        auto z = size(x) * 2;
        auto r = vector<word>(z);
        square_school<word>(r,x);

        // compute with gmp

        auto x_ = to_mpz(x);
        auto r_ = x_ * x_;

        // compare

        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error(fmt::format("r = {}, r_ = {}",format(r),format(r_)));
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleHegelTest,square_school_short)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        // full size
        auto fz = size(x) * 2;
        // short size
        auto sz = tc.draw(integers<size_t>({.max_value=fz-1}));

        // compute full result

        auto fr = vector<word>(fz);
        square_school<word>(fr,x);

        // compute short result

        auto sr = vector<word>(sz);
        square_school<word>(sr,x);

        // compare

        if ( ! equal( span(fr).subspan(0,sz), span(sr).subspan(0,sz) ) )
            throw runtime_error(fmt::format("fr = {}, sr = {}",format(fr),format(sr)));
    },
    { .test_cases = hegel_cases });
}
