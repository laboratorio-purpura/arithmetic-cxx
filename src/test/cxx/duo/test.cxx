// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(duo,is_zero)
{
    ASSERT_TRUE( is_zero( array { 0x00000000u, 0x00000000u } ) );

    ASSERT_FALSE( is_zero( array { 0x00000001u, 0x00000000u } ) );
    ASSERT_FALSE( is_zero( array { 0x00000000u, 0x00000001u } ) );
}

TEST(duo,not_zero)
{
    ASSERT_FALSE( not_zero( array { 0x00000000u, 0x00000000u } ) );

    ASSERT_TRUE( not_zero( array { 0x00000001u, 0x00000000u } ) );
    ASSERT_TRUE( not_zero( array { 0x00000000u, 0x00000001u } ) );
}

TEST(duo,is_odd)
{
    ASSERT_TRUE( is_odd( array { 0x00000001u, 0x00000000u } ) );

    ASSERT_FALSE( is_odd( array { 0x00000000u, 0x00000001u } ) );
}

TEST(duo,not_odd)
{
    ASSERT_FALSE( not_odd( array { 0x00000001u, 0x00000000u } ) );

    ASSERT_TRUE( not_odd( array { 0x00000000u, 0x00000001u } ) );
}

TEST(duo,is_normalized)
{
    ASSERT_TRUE( is_normalized( array { 0x00000000u, 0x80000000u } ) );

    ASSERT_FALSE( is_normalized( array { 0x80000000u, 0x00000000u } ) );
}

TEST(duo,not_normalized)
{
    ASSERT_FALSE( not_normalized( array { 0x00000000u, 0x80000000u } ) );

    ASSERT_TRUE( not_normalized( array { 0x80000000u, 0x00000000u } ) );
}