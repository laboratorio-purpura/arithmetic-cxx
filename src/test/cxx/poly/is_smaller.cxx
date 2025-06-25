// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <tuple>

#include <gtest/gtest.h>

import purple.arithmetic;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(poly,is_smaller_v0_0)
{
    ASSERT_EQ( is_smaller<unsigned>(v0,0U), 0 );
}

TEST(poly,is_smaller_v0_1)
{
    ASSERT_EQ( is_smaller<unsigned>(v0,1U), 1 );
}

TEST(poly,is_smaller_v1_0)
{
    ASSERT_EQ( is_smaller<unsigned>(v1,0U), 0 );
}

TEST(poly,is_smaller_v1_1)
{
    ASSERT_EQ( is_smaller<unsigned>(v1,1U), 0 );
}

TEST(poly,is_smaller_v1_2)
{
    ASSERT_EQ( is_smaller<unsigned>(v1,2U), 1 );
}
