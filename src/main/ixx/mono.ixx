// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <tuple>

export module purple.arithmetic:mono;

using std::array;
using std::tuple;

// Mono-degree arithmetic.

export namespace purple
{
    static_assert( sizeof(unsigned int) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// Properties.

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
        return 1U - is_smaller(x,y);
    }

    /// 1 if and only f x is greater than y, else 0.
    constexpr
    auto is_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1U - not_smaller(y,x);
    }

    /// 1 if and only f x is *not* greater than y, else 0.
    constexpr
    auto not_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1U - is_smaller(y,x);
    }

    /// count of "top" zeros.
    constexpr
    auto top_zeros ( unsigned x )
    // requires x != 0
    {
        return __builtin_clzg( x );
    }

    /// Operators.

    /// Sum and carry.
    constexpr
    auto sum ( unsigned int x, unsigned int y, unsigned int carry = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    /// Accumulate sum and return carry.
    constexpr
    auto sum_accumulate ( unsigned int & x, unsigned int y, unsigned int carry = 0 ) noexcept -> unsigned int
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    /// Sum modulo sizeof(x) * 8.
    auto sum_modulus ( unsigned int & x, unsigned int y ) -> unsigned int
    {
        return x + y;
    }

    /// Accumulate sum modulo sizeof(x) * 8.
    void sum_modulus_accumulate ( unsigned int & x, unsigned int y )
    {
        x += y;
    }

    /// Minus and carry.
    constexpr
    auto minus ( unsigned int x, unsigned int y, unsigned int carry = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_subc(x,y,carry,&carry);
        return { r, carry };
    }

    /// Accumulate minus and return carry.
    constexpr
    auto minus_accumulate ( unsigned int & x, unsigned int y, unsigned int carry = 0 ) noexcept -> unsigned int
    {
        x = __builtin_subc(x,y,carry,&carry);
        return carry;
    }

    /// Difference modulo sizeof(x) * 8.
    auto difference_modulus ( unsigned int x, unsigned int y ) -> unsigned int
    {
        return x - y;
    }

    /// Accumulate difference modulo sizeof(x) * 8.
    void difference_modulus_accumulate ( unsigned int & x, unsigned int y )
    {
        x -= y;
    }

    /// Product, low & high.
    constexpr
    auto product ( unsigned int x, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Product, accumulate low and return high.
    constexpr
    auto product_accumulate ( unsigned int & x, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        x = r;
        return r >> 32;
    }

    /// Product and sum, low & high.
    constexpr
    auto product_sum ( unsigned int x, unsigned int y, unsigned int z ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Product and sum, accumulate low and return high.
    constexpr
    auto product_sum_accumulate ( unsigned int & x, unsigned int y, unsigned int z ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        x = r;
        return r >> 32;
    }

    /// Twice N times, low and high.
    constexpr
    auto twice ( unsigned int x, unsigned N ) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) << N;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Twice N times, accumulate low and return high.
    constexpr
    auto twice_accumulate ( unsigned int & x, unsigned N ) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) << N;
        x = r;
        return r >> 32;
    }

    /// Twice N times and sum, low and high.
    constexpr
    auto twice_sum ( unsigned int x, unsigned N, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Twice N times and sum, accumulate low and return high.
    constexpr
    auto twice_sum_accumulate ( unsigned int & x, unsigned N, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        x = r;
        return r >> 32;
    }

    /// Half N times rounded down.
    constexpr
    auto half ( unsigned int & x, unsigned N ) noexcept -> unsigned int
    {
        return x >> N;
    }

    /// Accumulate half N times rounded down.
    constexpr
    void half_accumulate ( unsigned int & x, unsigned N ) noexcept
    {
        x >>= N;
    }

    /// Square, low and high.
    constexpr
    auto square ( unsigned int x ) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) * x;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Square, accumulate low and return high.
    constexpr
    auto square_accumulate ( unsigned int & x ) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) * x;
        x = r;
        return r >> 32;
    }

    /// Square and sum, low and high.
    constexpr
    auto square_sum ( unsigned int x, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Square and sum, accumulate low and return high.
    constexpr
    auto square_sum_accumulate ( unsigned int & x, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + y;
        x = r;
        return r >> 32;
    }

    /// Quotient and remainder.
    constexpr
    auto ratio ( unsigned int x, unsigned int y ) noexcept -> tuple< unsigned int, unsigned int >
    // requires 0 < y
    {
        assert( 0 < y );
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }

    /// Inverse approximation.
    auto inverse_nonzero ( unsigned y ) -> unsigned
    // requires 0 < y
    {
        assert( 0U < y );
        // 2^32 / y
        return 0xFFFFFFFFu / y;
    }

    /// Inverse approximation.
    auto inverse_normalised ( unsigned y ) -> unsigned
    // requires 2^31 <= y < 2^32
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        // ( ( ( 2^64 - 1 ) / y ) / 2^32 ) - 2^32
        auto x = static_cast< unsigned long long >( 0xFFFFFFFFU - y ) << 32 | 0xFFFFFFFFU;
        auto q = x / y;
        return q;
    }
}
