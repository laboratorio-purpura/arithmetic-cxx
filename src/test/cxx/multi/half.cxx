// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>
#include <tuple>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetics;
import purple.arithmetics.utility;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std::string_literals;

using std::array;
using std::ignore;
using std::span;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST_F(PurpleTest,half_assign_regression)
{
    {
        constexpr auto B = 32;
        using word = word<B>;

        auto x = array<word,2> { 0xA0EFBE01u, 0x00EFFDFFu, };
        auto qe = array<word,2> { 0xA0EFBE01u, 0x00EFFDFFu, };
        SCOPED_TRACE("expected: x = E800BF087FA040FF, z = 0 q = E800BF087FA040FF");

        auto q = array<word,5> {};
        auto r = array<word,5> {};
        halve<word>( q, x, 0uz );
        SCOPED_TRACE("result: q = " + format(q));

        ASSERT_GMP_EQ( to_mpz(qe), to_mpz(q) );
    }
}

TYPED_TEST(PurpleRandomTest,halve_64)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 100000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

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