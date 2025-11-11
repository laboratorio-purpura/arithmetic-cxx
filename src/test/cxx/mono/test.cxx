// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST_F(PurpleTest,is_normalized)
{
    ASSERT_TRUE( is_normalized( 0x80000000u ) );
    ASSERT_TRUE( is_normalized( 0xFFFFFFFFu ) );

    ASSERT_FALSE( is_normalized( 0x00000000u ) );
    ASSERT_FALSE( is_normalized( 0x7FFFFFFFu ) );
}

TEST_F(PurpleTest,not_normalized)
{
    ASSERT_FALSE( not_normalized( 0x80000000u ) );
    ASSERT_FALSE( not_normalized( 0xFFFFFFFFu ) );

    ASSERT_TRUE( not_normalized( 0x00000000u ) );
    ASSERT_TRUE( not_normalized( 0x7FFFFFFFu ) );
}