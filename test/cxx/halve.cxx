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
using namespace std;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleHegelTest,halve_differential_gmp)
{
    constexpr auto Bits = std::tuple_element<0,TypeParam>::type::value;

    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        auto x = tc.draw(vectors<word>(words<Bits>(),{.max_size=64}));
        auto y = tc.draw(integers<unsigned>({.max_value=Bits-1}));
        auto z = size(x);

        // compute with purple
        auto r = vector<word>(z);
        halve<word>(r,x,y);

        // compute with gmp
        auto x_ = to_mpz(x);
        auto r_ = x_ >> y;

        // compare
        if ( ::cmp(r_, to_mpz(r)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = 10000 });
}

TYPED_TEST(PurpleRandomTest,halve_64_differential_gmp)
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

        mpz_class gq = gx >> (B-1);

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "q = " + format(gq) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto q = array<word,64> {};

        ignore = halve<word>( q, x, (B-1) );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "q = " + format(q) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
    }
}
