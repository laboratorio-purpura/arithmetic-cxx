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

using std::ranges::max;
using std::runtime_error;
using std::span;
using std::vector;

namespace hg = hegel::generators;

using namespace purple::arithmetics;
using namespace purple::test;

TYPED_TEST(PurpleHegelTest,halve_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(hg::vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(hg::integers<unsigned>({.max_value=Bits-1}));
        auto z = size(x);

        // compute with purple
        auto r = vector<word>(z+1);
        r[z] = double_<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto r_ = x_ << y;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = 10000 });
}
