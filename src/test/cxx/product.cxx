#include <gtest/gtest.h>

import purple;
import purple.test;

using namespace purple;
using namespace purple::test;
using namespace std;

TEST(product,product_v0_v0)
{
    ASSERT_EQ( format( product(v0,v0) ), "000000000000000000000000" );
}

TEST(product,product_v0_v1)
{
    ASSERT_EQ( format( product(v0,v1) ), "000000000000000000000000" );
}

TEST(product,product_v0_v2)
{
    ASSERT_EQ( format( product(v0,v2) ), "000000000000000000000000" );
}

TEST(product,product_v0_vL)
{
    ASSERT_EQ( format( product(v0,vL) ), "000000000000000000000000" );
}

TEST(product,product_v0_vM)
{
    ASSERT_EQ( format( product(v0,vM) ), "000000000000000000000000" );
}

TEST(product,product_v1_v0)
{
    ASSERT_EQ( format( product(v1,v0) ), "000000000000000000000000" );
}

TEST(product,product_v1_v1)
{
    ASSERT_EQ( format( product(v1,v1) ), "000000000000000000000001" );
}

TEST(product,product_v1_v2)
{
    ASSERT_EQ( format( product(v1,v2) ), "000000000000000000000002" );
}

TEST(product,product_v1_vL)
{
    ASSERT_EQ( format( product(v1,vL) ), "000000000000000080000000" );
}

TEST(product,product_v1_vM)
{
    ASSERT_EQ( format( product(v1,vM) ), "0000000000000000FFFFFFFF" );
}

TEST(product,product_vL_v0)
{
    ASSERT_EQ( format( product(vL,v0) ), "000000000000000000000000" );
}

TEST(product,product_vL_v1)
{
    ASSERT_EQ( format( product(vL,v1) ), "000000000000000080000000" );
}

TEST(product,product_vL_v2)
{
    ASSERT_EQ( format( product(vL,v2) ), "000000000000000100000000" );
}

TEST(product,product_vL_vL)
{
    ASSERT_EQ( format( product(vL,vL) ), "000000004000000000000000" );
}

TEST(product,product_vL_vM)
{
    ASSERT_EQ( format( product(vL,vM) ), "000000007FFFFFFF80000000" );
}

TEST(product,product_vM_v0)
{
    ASSERT_EQ( format( product(vM,v0) ), "000000000000000000000000" );
}

TEST(product,product_vM_v1)
{
    ASSERT_EQ( format( product(vM,v1) ), "0000000000000000FFFFFFFF" );
}

TEST(product,product_vM_v2)
{
    ASSERT_EQ( format( product(vM,v2) ), "0000000000000001FFFFFFFE" );
}

TEST(product,product_vM_vL)
{
    ASSERT_EQ( format( product(vM,vL) ), "000000007FFFFFFF80000000" );
}

TEST(product,product_vM_vM)
{
    ASSERT_EQ( format( product(vM,vM) ), "00000000FFFFFFFE00000001" );
}

TEST(product,product_v00_v00)
{
    ASSERT_EQ( format( product(v00,v00) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v00_v01)
{
    ASSERT_EQ( format( product(v00,v01) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v00_v02)
{
    ASSERT_EQ( format( product(v00,v02) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v00_v0L)
{
    ASSERT_EQ( format( product(v00,v0L) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v00_v0M)
{
    ASSERT_EQ( format( product(v00,v0M) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v01_v00)
{
    ASSERT_EQ( format( product(v01,v00) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v01_v01)
{
    ASSERT_EQ( format( product(v01,v01) ), "0000000000000000000000010000000000000000" );
}

TEST(product,product_v01_v02)
{
    ASSERT_EQ( format( product(v01,v02) ), "0000000000000000000000020000000000000000" );
}

TEST(product,product_v01_v0L)
{
    ASSERT_EQ( format( product(v01,v0L) ), "0000000000000000800000000000000000000000" );
}

TEST(product,product_v01_v0M)
{
    ASSERT_EQ( format( product(v01,v0M) ), "0000000000000000FFFFFFFF0000000000000000" );
}

TEST(product,product_v0L_v00)
{
    ASSERT_EQ( format( product(v0L,v00) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v0L_v01)
{
    ASSERT_EQ( format( product(v0L,v01) ), "0000000000000000800000000000000000000000" );
}

TEST(product,product_v0L_v02)
{
    ASSERT_EQ( format( product(v0L,v02) ), "0000000000000001000000000000000000000000" );
}

TEST(product,product_v0L_v0L)
{
    ASSERT_EQ( format( product(v0L,v0L) ), "0000000040000000000000000000000000000000" );
}

TEST(product,product_v0L_v0M)
{
    ASSERT_EQ( format( product(v0L,v0M) ), "000000007FFFFFFF800000000000000000000000" );
}

TEST(product,product_v0M_v00)
{
    ASSERT_EQ( format( product(v0M,v00) ), "0000000000000000000000000000000000000000" );
}

TEST(product,product_v0M_v01)
{
    ASSERT_EQ( format( product(v0M,v01) ), "0000000000000000FFFFFFFF0000000000000000" );
}

TEST(product,product_v0M_v02)
{
    ASSERT_EQ( format( product(v0M,v02) ), "0000000000000001FFFFFFFE0000000000000000" );
}

TEST(product,product_v0M_v0L)
{
    ASSERT_EQ( format( product(v0M,v0L) ), "000000007FFFFFFF800000000000000000000000" );
}

TEST(product,product_v0M_v0M)
{
    ASSERT_EQ( format( product(v0M,v0M) ), "00000000FFFFFFFE000000010000000000000000" );
}

TEST(product,product_D2836A67C068B195_0986A2ABBA94FA4D)
{
    auto const x = vector { 0xC068B195U, 0xD2836A67U };
    auto const y = vector { 0xBA94FA4DU, 0x0986A2ABU };
    ASSERT_EQ( format( product(x,y) ), "0000000007D5554BB89E1D378484892F090CEBD1" );
}
