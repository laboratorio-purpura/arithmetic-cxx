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

using std::array;
using std::ranges::max;
using std::runtime_error;
using std::span;
using std::vector;

namespace hg = hegel::generators;

using namespace purple::arithmetics;
using namespace purple::test;

TYPED_TEST(PurpleHegelTest,subtract_N_1_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.min_size=1,.max_size=64}));
        auto y = tc.draw(words<Bits>());

        // TODO: lift this restriction
        if ( is_smaller<word>(x,array{y}) )
            std::swap(x[0],y); // TODO

        // compute with purple
        auto z = size(x);
        auto r = vector<word>(z);
        auto b = subtract<word>(r,x,y);
        assert(b == 0);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ - y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = 10000 });
}


TYPED_TEST(PurpleHegelTest,subtract_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(hg::vectors<word>(words<Bits>(),{.max_size=64}));

        // TODO: lift this restriction
        if ( is_smaller<word>(x,y) )
            swap(x,y);

        // compute with purple
        auto z = max(size(x),size(y));
        auto r = vector<word>(z);
        auto b = subtract<word>(r,x,y);
        assert(b == 0);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto y_ = to_mpz(y);
        auto r_ = x_ - y_;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = 10000 });
}
