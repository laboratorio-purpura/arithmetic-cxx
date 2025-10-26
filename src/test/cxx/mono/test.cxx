// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(mono,is_zero)
{
    ASSERT_TRUE( is_zero( 0x00000000u ) );

    ASSERT_FALSE( is_zero( 0x00000001u ) );
    ASSERT_FALSE( is_zero( 0x00000002u ) );
    ASSERT_FALSE( is_zero( 0x80000000u ) );
    ASSERT_FALSE( is_zero( 0xFFFFFFFFu ) );
}

TEST(mono,not_zero)
{
    ASSERT_FALSE( not_zero( 0x00000000u ) );

    ASSERT_TRUE( not_zero( 0x00000001u ) );
    ASSERT_TRUE( not_zero( 0x00000002u ) );
    ASSERT_TRUE( not_zero( 0x80000000u ) );
    ASSERT_TRUE( not_zero( 0xFFFFFFFFu ) );
}

TEST(mono,is_odd)
{
    ASSERT_TRUE( is_odd( 0x00000001u ) );
    ASSERT_TRUE( is_odd( 0xFFFFFFFFu ) );

    ASSERT_FALSE( is_odd( 0x00000000u ) );
    ASSERT_FALSE( is_odd( 0x00000002u ) );
    ASSERT_FALSE( is_odd( 0xFFFFFFFEu ) );
}

TEST(mono,not_odd)
{
    ASSERT_FALSE( not_odd( 0x00000001u ) );
    ASSERT_FALSE( not_odd( 0xFFFFFFFFu ) );

    ASSERT_TRUE( not_odd( 0x00000000u ) );
    ASSERT_TRUE( not_odd( 0x00000002u ) );
    ASSERT_TRUE( not_odd( 0xFFFFFFFEu ) );
}

TEST(mono,is_normalized)
{
    ASSERT_TRUE( is_normalized( 0x80000000u ) );
    ASSERT_TRUE( is_normalized( 0xFFFFFFFFu ) );

    ASSERT_FALSE( is_normalized( 0x00000000u ) );
    ASSERT_FALSE( is_normalized( 0x7FFFFFFFu ) );
}

TEST(mono,not_normalized)
{
    ASSERT_FALSE( not_normalized( 0x80000000u ) );
    ASSERT_FALSE( not_normalized( 0xFFFFFFFFu ) );

    ASSERT_TRUE( not_normalized( 0x00000000u ) );
    ASSERT_TRUE( not_normalized( 0x7FFFFFFFu ) );
}