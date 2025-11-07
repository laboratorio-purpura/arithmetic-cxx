// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <random>
#include <span>

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

// Assertions.

#define ASSERT_GMP_EQ(x,y) ASSERT_EQ( ::cmp( x, y ), 0 )

// Tests.

template <typename Word, size_t XZ>
void division_assign_test ( array<Word,XZ> const & x, Word y )
{
    // compute with purple
    auto q = array<unsigned,XZ> {};
    auto r = division_assign<unsigned>( q, x, y );

    // compute with gmp
    auto gx = mpz_class( format(x), 16 );
    auto gq = gx / y;
    auto gr = gx % y;

    // compare
    SCOPED_TRACE( std::string() +
        "purple:\n" +
        "x = " + format(x) + "\n" +
        "y = " + format(y) + "\n" +
        "q = " + format(q) + "\n" +
        "r = " + format(r) + "\n" +
        "gmp:\n" +
        "x = " + format(gx) + "\n" +
        "y = " + format(y) + "\n" +
        "q = " + format(gq) + "\n" +
        "r = " + format(gr) + "\n"
    );
    ASSERT_GMP_EQ(
        gq,
        mpz_class( format(q), 16 )
    );
    ASSERT_GMP_EQ(
        gr,
        mpz_class( format(r), 16 )
    );
}

TEST(poly,division_N_1)
{
    division_assign_test(
        array { 0x00000000u, 0xFFFFFFFFu },
        0x80000000u
    );
}

TEST(poly,division_64_1_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64uz> {};
        std::ranges::generate(x,ref(generator));

        auto y = 0u;
        while ( is_zero( y ) )
            y = generator();

        // compute with purple
        auto q = array<unsigned,65uz> {};
        auto r = division_assign<unsigned>( q, x, y );

        // compute with gmp
        auto gx = mpz_class( format(x), 16 );
        auto gy = mpz_class( format(y), 16 );
        auto gq = gx / gy;
        auto gr = gx % gy;

        // compare
        SCOPED_TRACE( std::string() +
            "purple:\n" +
            "x = " + format(x) + "\n" +
            "y = " + format(y) + "\n" +
            "q = " + format(q) + "\n" +
            "r = " + format(r) + "\n" +
            "gmp:\n" +
            "x = " + format(gx) + "\n" +
            "y = " + format(gy) + "\n" +
            "q = " + format(gq) + "\n" +
            "r = " + format(gr) + "\n"
        );
        ASSERT_GMP_EQ(
            gq,
            mpz_class( format(q), 16 )
        );
        ASSERT_GMP_EQ(
            gr,
            mpz_class( format(r), 16 )
        );
    }
}

template <typename Word, size_t XZ, size_t YZ>
void division_assign_test ( array<Word,XZ> const & x, array<Word,YZ> const & y )
{
    // compute with purple
    auto q = array<unsigned,XZ> {};
    auto r = array<unsigned,XZ+1> {};
    division_assign<unsigned>( q, r, x, y );

    // compute with gmp
    auto gx = mpz_class( format(x), 16 );
    auto gy = mpz_class( format(y), 16 );
    auto gq = gx / gy;
    auto gr = gx % gy;

    // compare
    SCOPED_TRACE( std::string() +
        "purple:\n" +
        "x = " + format(x) + "\n" +
        "y = " + format(y) + "\n" +
        "q = " + format(q) + "\n" +
        "r = " + format(r) + "\n" +
        "gmp:\n" +
        "x = " + format(gx) + "\n" +
        "y = " + format(gy) + "\n" +
        "q = " + format(gq) + "\n" +
        "r = " + format(gr) + "\n"
    );
    ASSERT_GMP_EQ(
        gq,
        mpz_class( format(q), 16 )
    );
    ASSERT_GMP_EQ(
        gr,
        mpz_class( format(r), 16 )
    );
}

TEST(poly,division_M_N)
{
    division_assign_test(
        array { 0x00000000u, 0xFFFFFFFFu },
        array { 0x80000000u }
    );
}

TEST(poly,division_64_32_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0; i != 1000; ++i)
    {
        auto x = array<unsigned,64uz> {};
        std::ranges::generate(x,ref(generator));

        auto y = array<unsigned,32uz> {};
        while ( is_zero<unsigned>( y ) )
           std::ranges::generate(y,ref(generator));

        division_assign_test( x, y );
    }
}