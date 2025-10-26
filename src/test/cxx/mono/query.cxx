// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(mono,leading_zeros)
{
    ASSERT_EQ( leading_zero_bits( 0x00000001u ), 31 );
    ASSERT_EQ( leading_zero_bits( 0x80000000u ), 0  );
    ASSERT_EQ( leading_zero_bits( 0xFFFFFFFFu ), 0  );
}