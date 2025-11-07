// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>
#include <vector>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;

using std::array;
using std::random_device;
using random_integer = std::linear_congruential_engine<unsigned, 48271UL, 0UL, 2147483647UL>;
using std::span;
using std::vector;

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Tests.

template <typename Word, size_t XZ>
void sum_assign_test ( array<Word,XZ> const & x, Word y )
{
    // compute with purple
    auto r = array<unsigned,XZ+1> {};
    r[XZ] = sum_assign<unsigned>( r, x, y );

    // compute with gmp
    auto gx = mpz_class( format(x), 16 );
    auto gr = gx + y;

    // compare
    SCOPED_TRACE( std::string() +
        "purple:\n" +
        "x = " + format(x) + "\n" +
        "y = " + format(y) + "\n" +
        "r = " + format(r) + "\n" +
        "gmp:\n" +
        "x = " + format(gx) + "\n" +
        "y = " + format(y) + "\n" +
        "r = " + format(gr) + "\n"
    );
    ASSERT_GMP_EQ(
        gr,
        mpz_class( format(r), 16 )
    );
}

TEST(poly,sum_assign_64_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        auto x = array<unsigned,64uz> {};
        std::ranges::generate(x,ref(generator));

        auto y = generator();

        sum_assign_test( x, y );
    }
}

template <typename Word, size_t XZ, size_t YZ>
void sum_assign_test ( array<Word,XZ> const & x, array<Word,YZ> const & y )
{
    static_assert( XZ >= YZ );

    // compute with purple
    auto r = array<unsigned,XZ+1> {};
    r[XZ] = sum_assign<unsigned>( r, x, y );

    // compute with gmp
    auto gx = mpz_class( format(x), 16 );
    auto gy = mpz_class( format(y), 16 );
    auto gr = gx + gy;

    // compare
    SCOPED_TRACE( std::string() +
        "purple:\n" +
        "x = " + format(x) + "\n" +
        "y = " + format(y) + "\n" +
        "r = " + format(r) + "\n" +
        "gmp:\n" +
        "x = " + format(gx) + "\n" +
        "y = " + format(gy) + "\n" +
        "r = " + format(gr) + "\n"
    );
    ASSERT_GMP_EQ(
        gr,
        mpz_class( format(r), 16 )
    );
}

TEST(poly,sum_assign_32_32_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        auto x = array<unsigned,32uz> {};
        std::ranges::generate(x,ref(generator));

        auto y = array<unsigned,32uz> {};
        std::ranges::generate(y,ref(generator));

        sum_assign_test( x, y );
    }
}
