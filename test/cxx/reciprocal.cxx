// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

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
using namespace std;
using namespace std::string_literals;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TYPED_TEST(PurpleHegelTest,reciprocal_normal_2_differential_gmp)
{
    constexpr auto Bits = tuple_element<0,TypeParam>::type::value;
    using word = word<Bits>;

    hegel::test([](hegel::TestCase& tc)
    {
        // generate with hegel
        // TODO: y must be "normal"; how can we improve this?
        constexpr auto normal = typename word::type(1) << (Bits-1);
        auto y = tc.draw(vectors<word>(words<Bits>(),{.min_size=2,.max_size=2}));
        y[1].v |= normal;

        // compute with purple
        auto iy = reciprocal_normalized<word,2>( span<word,2>(y) );

        // compute with gmp
        mpz_class y_ = to_mpz(y);
        mpz_class base = mpz_class(1) << Bits;
        mpz_class iy_ = ( ( base * base * base - 1u ) / y_ ) - base;

        // compare
        if ( ::cmp(iy_, to_mpz(iy)) != 0 )
            throw runtime_error("GMP and purple differ");
    },
    { .test_cases = hegel_cases });
}

TYPED_TEST(PurpleRandomTest,reciprocal_normal_2_differential_gmp)
{
    constexpr auto B = tuple_element<0,TypeParam>::type::value;
    using generator = tuple_element<1,TypeParam>::type;
    using word = word<B>;

    for (auto i = 0uz; i != random_cases; ++i)
    {
        SCOPED_TRACE("i = " + fmt::format("{}",i));

        // compute with gmp

        mpz_class gy {};
        generator::generate(gy,2,B);
        mpz_setbit(gy.get_mpz_t(),(2*B)-1);

        mpz_class gb = mpz_class(1) << B;
        mpz_class giy = ( ( gb * gb * gb - 1u ) / gy ) - gb;

        SCOPED_TRACE("gmp:\n"s +
            "y = " + format(gy) + "\n" +
            "iy = " + format(giy) + "\n"
        );

        // compute with purple

        auto y = array<word,2> {};
        assign(y,gy);

        auto iy = reciprocal_normalized<word,2>( y );

        SCOPED_TRACE("purple:\n"s +
            "y = " + format(y) + "\n" +
            "iy = " + format(iy) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( giy, to_mpz(iy) );
    }
}