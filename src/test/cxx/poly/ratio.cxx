// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <array>
#include <span>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;

using std::array;
using std::span;

TEST(poly,ratio_normalised_v00_80000000)
{
    auto x = v00;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalised(y);
    auto r = ratio_normalised<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "0000000000000000" );
    ASSERT_EQ( format(r), "00000000" );
}

TEST(poly,ratio_normalised_vMM_80000000)
{
    auto x = vMM;
    auto y = 0x80000000U;
    auto q = array<unsigned,2>();
    auto iy = inverse_normalised(y);
    auto r = ratio_normalised<unsigned>( span(q), span(x), y, iy );
    ASSERT_EQ( format(q), "00000001FFFFFFFF" );
    ASSERT_EQ( format(r), "7FFFFFFF" );
}
