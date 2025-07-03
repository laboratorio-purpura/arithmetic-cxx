// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <tuple>

export module purple.arithmetic:mono;

using std::array;
using std::size_t;
using std::tuple;

// Scalar arithmetic.

export namespace purple
{
    static_assert( sizeof(unsigned int) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// Properties.

    /// Inverse approximation.
    auto inverse ( unsigned y ) noexcept -> unsigned
    // requires y != 0
    {
        // 2^32 / y
        return 0xFFFFFFFFu / y;
    }

    /// Inverse approximation of "normalised" integer.
    auto inverse_normalised ( unsigned y ) noexcept -> unsigned
    // requires 2^31 <= y < 2^32
    {
        // ( ( ( 2^64 - 1 ) / y ) / 2^32 ) - 2^32
        auto x = static_cast< unsigned long long >( 0xFFFFFFFFU - y ) << 32 | 0xFFFFFFFFU;
        auto q = x / y;
        return q;
    }

    /// 1 if and only if x is zero, else 0.
    constexpr
    auto is_zero ( unsigned int x ) noexcept -> unsigned int
    {
        x |= -x;
        x >>= 31U;
        return 1U - x;
    }

    /// 1 if and only if x is *not* zero, else 0.
    constexpr
    auto not_zero ( unsigned int x ) noexcept -> unsigned int
    {
        x |= -x;
        return x >> 31U;
    }

    /// Relations.

    /// 1 if and only f x is smaller than y, else 0.
    constexpr
    auto is_smaller ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        auto r0 = x ^ y;
        auto r1 = x - y;
        r1 ^= r0 & (r1 ^ x ^ (1U << 31));
        return r1 >> 31;
    }

    /// 1 if and only f x is *not* smaller than y, else 0.
    constexpr
    auto not_smaller ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1U - is_smaller( x, y );
    }

    /// 1 if and only f x is greater than y, else 0.
    constexpr
    auto is_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return is_smaller( y, x );
    }

    /// 1 if and only f x is *not* greater than y, else 0.
    constexpr
    auto not_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1U - is_smaller( y, x );
    }

    /// count of "top" zeros.
    constexpr
    auto top_zeros ( unsigned x ) noexcept
    // requires x != 0
    {
        return __builtin_clzg( x );
    }

    /// Expansion operators.

    /// Sum with carry.
    constexpr
    auto sum ( unsigned int x, unsigned int y, unsigned int carry = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    /// Accumulate sum, return carry.
    constexpr
    auto sum_accumulate ( unsigned int & x, unsigned int y, unsigned int carry = 0 ) noexcept -> unsigned int
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    /// Nth twice with carry.
    constexpr
    auto twice ( unsigned int x, size_t N ) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) << N;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Accumulate Nth twice, return carry.
    constexpr
    auto twice_accumulate ( unsigned int & x, size_t N ) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) << N;
        x = r;
        return r >> 32;
    }

    /// Sum of Nth twice with carry.
    constexpr
    auto twice_sum ( unsigned int x, size_t N, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Accumulate sum of Nth twice, return carry.
    constexpr
    auto twice_sum_accumulate ( unsigned int & x, size_t N, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        x = r;
        return r >> 32;
    }

    /// Product with carry.
    constexpr
    auto product ( unsigned int x, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Accumulate product, return carry.
    constexpr
    auto product_accumulate ( unsigned int & x, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        x = r;
        return r >> 32;
    }

    /// Product and sum with carry.
    constexpr
    auto product_sum ( unsigned int x, unsigned int y, unsigned int z ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Accumulate product and sum, return carry.
    constexpr
    auto product_sum_accumulate ( unsigned int & x, unsigned int y, unsigned int z ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        x = r;
        return r >> 32;
    }

    /// Reduction operators.

    /// Difference with borrow.
    constexpr
    auto difference ( unsigned int x, unsigned int y, unsigned int borrow = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_subc(x,y,borrow,&borrow);
        return { r, borrow };
    }

    /// Accumulate difference, return borrow.
    constexpr
    auto difference_accumulate ( unsigned int & x, unsigned int y, unsigned int borrow = 0 ) noexcept -> unsigned int
    {
        x = __builtin_subc(x,y,borrow,&borrow);
        return borrow;
    }

    /// Nth half rounded down.
    constexpr
    auto half ( unsigned int & x, size_t N ) noexcept -> unsigned int
    {
        return x >> N;
    }

    /// Accumulate Nth half rounded down.
    constexpr
    void half_accumulate ( unsigned int & x, size_t N ) noexcept
    {
        x >>= N;
    }

    /// Quotient and remainder.
    constexpr
    auto ratio ( unsigned int x, unsigned int y ) noexcept -> tuple< unsigned int, unsigned int >
    // requires y != 0
    {
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }
}
