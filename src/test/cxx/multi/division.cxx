// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>
#include <tuple>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.arithmetic.utility;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std::string_literals;

using std::array;
using std::ignore;
using std::span;

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

TEST_F(PurpleTest,divide_regression)
{
    {
        constexpr auto B = 32;
        using word = word<B>;

        SCOPED_TRACE("expected: x = FFFFFE00FFFFFE007FFFC000, y = FFFFFFFFFF0FFFFF, q = FFFFFE01, r = EFFDFFA0EFBE01");
        auto x = array<word,3> { 0x7FFFC000u, 0xFFFFFE00u, 0xFFFFFE00u, };
        auto y = array<word,2> { 0xFF0FFFFFu, 0xFFFFFFFFu, };
        auto qe = array<word,1> { 0xFFFFFE01u, };
        auto re = array<word,2> { 0xA0EFBE01u, 0x00EFFDFFu, };

        auto q = array<word,5> {};
        auto r = array<word,5> {};
        divide<word>( q, r, x, y );

        SCOPED_TRACE("result: q = " + format(q) + ", r = " + format(r));
        ASSERT_GMP_EQ( to_mpz(qe), to_mpz(q) );
        ASSERT_GMP_EQ( to_mpz(re), to_mpz(r) );
    }
    {
        constexpr auto B = 32;
        using word = word<B>;

        SCOPED_TRACE("expected: x = FFE0000007FFFFFF80000000, y = FFFFFF0000003FFF, q = FFE000FF, r = E800BF087FA040FF");
        auto x = array<word,3> { 0x80000000u, 0x07FFFFFFu, 0xFFE00000u, };
        auto y = array<word,2> { 0x00003FFFu, 0xFFFFFF00u, };
        auto qe = array<word,1> { 0xFFE000FFu, };
        auto re = array<word,2> { 0x7FA040FFu, 0xE800BF08, };

        auto q = array<word,5> {};
        auto r = array<word,5> {};
        divide<word>( q, r, x, y );

        SCOPED_TRACE("result: q = " + format(q) + ", r = " + format(r));
        ASSERT_GMP_EQ( to_mpz(qe), to_mpz(q) );
        ASSERT_GMP_EQ( to_mpz(re), to_mpz(r) );
    }
}

TYPED_TEST(PurpleRandomTest,divide_64_1)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 1000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        mpz_class gy {};
        while (gy == 0)
            generator::generate(gy,1,B);

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n"
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto y = array<word,1> {};
        assign(y,gy);

        auto q = array<word,64> {};

        auto r = divide<word>( q, x, y[0] );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleRandomTest,divide_normal_strict_N1_N)
{
    constexpr auto N = 4;
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator::generate(gy,N,B);
        mpz_setbit(gy.get_mpz_t(),(N*B)-1);

        mpz_class gx {};
        generator::generate(gx,N+1,B);
        if ((gx >> B) >= gy)
            gx = gx - (gy << B);
        assert( (gx >> B) < gy );

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,N+1> {};
        assign(x,gx);

        auto y = array<word,N> {};
        assign(y,gy);

        auto iy = reciprocal_normalized<word>( y[N-1] );

        auto r = array<word,N+1> {};

        auto q = divide_normal_strict<word>( r, x, y, iy );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleRandomTest,divide_normal_strict_33_32)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator::generate(gy,32,B);
        mpz_setbit(gy.get_mpz_t(),(32*B)-1);

        mpz_class gx {};
        generator::generate(gx,33,B);
        if ((gx >> B) >= gy)
            gx = gx - (gy << B);
        assert( (gx >> B) < gy );

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,33> {};
        assign(x,gx);

        auto y = array<word,32> {};
        assign(y,gy);

        auto iy = reciprocal_normalized<word>( y[31] );

        auto r = array<word,33> {};

        auto q = divide_normal_strict<word>( r, x, y, iy );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}

TYPED_TEST(PurpleRandomTest,divide_64_32)
{
    constexpr auto B = std::tuple_element<0,TypeParam>::type::value;
    using generator = std::tuple_element<1,TypeParam>::type;

    using word = word<B>;

    for (auto i = 0; i != 1000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gx {};
        generator::generate(gx,64,B);

        mpz_class gy {};
        while (gy == 0)
            generator::generate(gy,32,B);

        mpz_class gq = gx / gy;
        mpz_class gr = gx % gy;

        SCOPED_TRACE("gmp:\n"s +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );

        // compute with purple

        auto x = array<word,64> {};
        assign(x,gx);

        auto y = array<word,32> {};
        assign(y,gy);

        if ( is_zero( y[31] ) ) // TODO
            continue;

        auto q = array<word,65> {};

        auto r = array<word,65> {};

        divide<word>( q, r, x, y );

        SCOPED_TRACE("purple:\n"s +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( gx, to_mpz(x) );
        ASSERT_GMP_EQ( gy, to_mpz(y) );
        ASSERT_GMP_EQ( gq, to_mpz(q) );
        ASSERT_GMP_EQ( gr, to_mpz(r) );
    }
}