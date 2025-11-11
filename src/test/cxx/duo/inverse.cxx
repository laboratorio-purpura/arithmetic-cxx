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

TEST_F(PurpleTest,reciprocal_normalised_2_9C8739F05AA157B2)
{
    auto y = array { 0x5AA157B2U, 0x9C8739F0U };
    auto iy = reciprocal_normalized( y );
    ASSERT_EQ( format(iy), "A2AF5356" );
}

TEST_P(PurpleRandomTest,reciprocal_normalised_2)
{
    constexpr auto B = sizeof(unsigned) * 8;
    using word = unsigned;

    auto generator = GetParam();

    for (auto i = 0uz; i != 10000; ++i)
    {
        SCOPED_TRACE("i = " + format(i));

        // compute with gmp

        mpz_class gy {};
        generator->generate(gy,2,B);
        mpz_setbit(gy.get_mpz_t(),(2*B)-1);

        mpz_class giy = ( mpz_class("FFFFFFFFFFFFFFFFFFFFFFFF",16) / gy ) - mpz_class("100000000",16);

        SCOPED_TRACE("gmp:\n"s +
            "y = " + format(gy) + "\n" +
            "iy = " + format(giy) + "\n"
        );

        // compute with purple

        auto y = array<word,2> {};
        assign(y,gy);

        auto iy = reciprocal_normalized( y );

        SCOPED_TRACE("purple:\n"s +
            "y = " + format(y) + "\n" +
            "iy = " + format(iy) + "\n"
        );

        // compare

        ASSERT_GMP_EQ( giy, to_mpz(iy) );
    }
}
