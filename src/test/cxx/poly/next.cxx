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
void next_assign_test ( array<Word,XZ> const & x )
{
    // compute with purple
    auto r = array<unsigned,XZ+1> {};
    r[XZ] = next_assign<unsigned>( r, x );

    // compute with gmp
    auto gx = mpz_class( format(x), 16 );
    auto gr = ++gx;

    // compare
    SCOPED_TRACE( std::string() +
        "purple:\n" +
        "x = " + format(x) + "\n" +
        "r = " + format(r) + "\n" +
        "gmp:\n" +
        "x = " + format(gx) + "\n" +
        "r = " + format(gr) + "\n"
    );
    ASSERT_GMP_EQ(
        gr,
        mpz_class( format(r), 16 )
    );
}

TEST(poly,next_assign_32_random)
{
    random_device random;
    random_integer generator { random() };

    for (auto i = 0uz; i != 1000; ++i)
    {
        auto x = array<unsigned,32uz> {};
        std::ranges::generate(x,ref(generator));

        next_assign_test( x );
    }
}
