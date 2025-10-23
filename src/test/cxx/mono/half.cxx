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

TEST(mono,half_3)
{
    auto [ q, r ] = half(3u,1);
    ASSERT_EQ( q, 1 );
    ASSERT_EQ( r, 1 );
}

TEST(mono,half_7)
{
    auto [ q, r ] = half(7u,1);
    ASSERT_EQ( q, 3 );
    ASSERT_EQ( r, 1 );
}