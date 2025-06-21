// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(mono,not_zero_0)
{
    ASSERT_EQ( not_zero(0U), 0 );
}

TEST(mono,not_zero_1)
{
    ASSERT_EQ( not_zero(1U), 1 );
}

TEST(mono,not_zero_2)
{
    ASSERT_EQ( not_zero(2U), 1 );
}

TEST(mono,not_zero_L)
{
    ASSERT_EQ( not_zero(L), 1 );
}

TEST(mono,not_zero_M)
{
    ASSERT_EQ( not_zero(M), 1 );
}