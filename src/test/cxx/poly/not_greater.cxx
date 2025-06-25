// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly,not_greater_v0_0)
{
    ASSERT_EQ( not_greater<unsigned>(v0,0U), 1 );
}

TEST(poly,not_greater_v0_1)
{
    ASSERT_EQ( not_greater<unsigned>(v0,1U), 1 );
}

TEST(poly,not_greater_v1_0)
{
    ASSERT_EQ( not_greater<unsigned>(v1,0U), 0 );
}

TEST(poly,not_greater_v1_1)
{
    ASSERT_EQ( not_greater<unsigned>(v1,1U), 1 );
}
