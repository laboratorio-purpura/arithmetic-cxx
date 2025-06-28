// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(mono,inverse_nonzero_00000001)
{
    auto y = 0x00000001U;
    auto iy = inverse_nonzero(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST(mono,inverse_nonzero_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = inverse_nonzero(y);
    ASSERT_EQ( format(iy), "00000001" );
}

TEST(mono,inverse_normalised_80000000)
{
    auto y = 0x80000000U;
    auto iy = inverse_normalised(y);
    ASSERT_EQ( format(iy), "FFFFFFFF" );
}

TEST(mono,inverse_normalised_FFFFFFFF)
{
    auto y = 0xFFFFFFFFU;
    auto iy = inverse_normalised(y);
    ASSERT_EQ( format(iy), "00000001" );
}
