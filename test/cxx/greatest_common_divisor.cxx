// Copyright (c) 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
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

TYPED_TEST(PurpleHegelTest,greatest_common_divisor_clobber_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));

        // compute with gmp

        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = ::gcd(x_, y_);

        // compute with purple — clobbers x and y

        auto r = greatest_common_divisor_clobber<word>( x, y );

        // compare

        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error(fmt::format("r = {}, r_ = {}",format(r),format(r_)));
    },
    { .test_cases = hegel_cases });
}


TYPED_TEST(PurpleHegelTest,greatest_common_divisor_differential)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel

        auto x = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto z = max(size(x),size(y));

        // compute with purple

        auto r = vector<word>(z);
        greatest_common_divisor<word>(r,x,y);

        // compute with gmp

        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = ::gcd(x_, y_);

        // compare

        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error(fmt::format("r = {}, r_ = {}",format(r),format(r_)));
    },
    { .test_cases = hegel_cases });
}
