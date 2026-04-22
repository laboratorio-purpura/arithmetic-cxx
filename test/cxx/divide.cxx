// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

#include <gmpxx.h>
#include <gtest/gtest.h>
#include <hegel/hegel.h>

import purple.arithmetics;
import purple.arithmetics.utility;
import purple.test;

using std::ignore;
using std::ranges::max;
using std::runtime_error;
using std::span;
using std::tie;
using std::vector;

namespace hg = hegel::generators;

using namespace purple::arithmetics;
using namespace purple::test;

TYPED_TEST(PurpleHegelTest,divide_normal_strict_2_1_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        // TODO: y must be "normal"; how can we improve this?
        constexpr auto normal = typename word::type(1) << (Bits-1);
        auto y = tc.draw(words<Bits>({.min_value=normal}));
        // TODO: x must be "strict"; how can we improve this?
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=2,.max_size=2}));
        if ( not_smaller(x[1],y) )
            tie( x[1], ignore ) = difference(x[1],y,word{0});

        // compute with purple
        auto iy = reciprocal_normalized(y);
        auto [q,r] = divide_normal_strict<word,2>( span<word,2>(x), y, iy );

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto q_ = x_ / y_;
        auto r_ = x_ % y_;

        // compare
        if ( ::cmp(q_, to_mpz(q)) != 0 )
            throw runtime_error("GMP and purple differ on quotient");
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ on remainder");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleHegelTest,divide_N_1_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(words<Bits>({.min_value=1}));

        // compute with purple
        auto r = vector<word>(size(x));
        auto q = divide<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto q_ = x_ / y_;
        auto r_ = x_ % y_;

        // compare
        if ( ::cmp(q_, to_mpz(q)) != 0 )
            throw runtime_error("GMP and purple differ on quotient");
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ on remainder");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleHegelTest,divide_normal_strict_3_2_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        // TODO: y must be "normal"; how can we improve this?
        constexpr auto normal = typename word::type(1) << (Bits-1);
        auto y = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=2,.max_size=2}));
        y[1].v |= normal;
        // TODO: x must be "strict"; how can we improve this?
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=3,.max_size=3}));
        if ( not_smaller<word>( span(x).subspan(1), y ) )
            subtract<word>( span(x).subspan(1), span(x).subspan(1), y );

        // compute with purple
        auto iy = reciprocal_normalized<word,2>( span<word,2>(y) );
        auto [q,r] = divide_normal_strict<word,3,2>( span<word,3>(x), span<word,2>(y), iy );

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto q_ = x_ / y_;
        auto r_ = x_ % y_;

        // compare
        if ( ::cmp(q_, to_mpz(q)) != 0 )
            throw runtime_error("GMP and purple differ on quotient");
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ on remainder");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleHegelTest,divide_normal_strict_M_N_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        // TODO: y must be "normal"; how can we improve this?
        constexpr auto normal = typename word::type(1) << (Bits-1);
        auto y = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=2,.max_size=63}));
        y[size(y)-1].v |= normal;
        // TODO: x must be "strict"; how can we improve this?
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=size(y)+1,.max_size=size(y)+1}));
        if ( not_smaller<word>( span(x).subspan(1), y ) )
            subtract<word>( span(x).subspan(1), span(x).subspan(1), y );

        // compute with purple
        auto r = vector<word>(size(x));
        auto iy = reciprocal_normalized( y[size(y)-1] );
        auto q = divide_normal_strict<word>( span(r), span(x), span(y), iy );

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto q_ = x_ / y_;
        auto r_ = x_ % y_;

        // compare
        if ( ::cmp(q_, to_mpz(q)) != 0 )
            throw runtime_error("GMP and purple differ on quotient");
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ on remainder");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleHegelTest,divide_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto y = tc.draw(hg::vectors<word>(words<Bits>({.min_value=1}), // TODO: lift min_value restriction
            {.min_size=2,.max_size=63})); // TODO: lift min_size restriction
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),
            {.min_size=size(y)+1,.max_size=64})); // TODO: lift min_size restriction

        // compute with purple
        auto z = size(x);
        auto q = vector<word>(z);
        auto r = vector<word>(z+1); // TODO: lift +1 requirement
        divide<word>(q,r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto q_ = x_ / y_;
        auto r_ = x_ % y_;

        // compare
        if ( ::cmp(q_, to_mpz(q)) != 0 )
            throw runtime_error("GMP and purple differ on quotient");
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ on remainder");
    },
    { .test_cases = 10000 });
}
