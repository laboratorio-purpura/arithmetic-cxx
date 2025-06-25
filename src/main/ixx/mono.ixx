// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>

export module purple.arithmetic:mono;

using std::array;

export namespace purple
{
    static_assert( sizeof(unsigned int) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// mono integer properties

    /// 1 if and only if x is zero, else 0.
    constexpr
    auto is_zero (unsigned int x) noexcept -> unsigned int
    {
        x |= -x;
        x >>= 31U;
        return 1 - x;
    }

    /// 1 if and only if x is *not* zero, else 0.
    constexpr
    auto not_zero (unsigned int x) noexcept -> unsigned int
    {
        x |= -x;
        return x >> 31U;
    }

    // 1 if and only f x is smaller than y, else 0.
    constexpr
    auto is_smaller (unsigned int x, unsigned int y) noexcept -> unsigned int
    {
        auto r0 = x ^ y;
        auto r1 = x - y;
        r1 ^= r0 & (r1 ^ x ^ (1U << (32-1)));
        return r1 >> 31;
    }

    /// mono integer arithmetic

    constexpr
    auto sum (unsigned int x, unsigned int y, unsigned int carry = 0) noexcept -> array< unsigned int, 2 >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    constexpr
    auto sum_accumulate (unsigned int & x, unsigned int y, unsigned int carry = 0) noexcept -> unsigned int
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    constexpr
    auto minus (unsigned int x, unsigned int y, unsigned int carry = 0) noexcept -> array< unsigned int, 2 >
    {
        auto r = __builtin_subc(x,y,carry,&carry);
        return { r, carry };
    }

    constexpr
    auto minus_accumulate (unsigned int & x, unsigned int y, unsigned int carry = 0) noexcept -> unsigned int
    {
        x = __builtin_subc(x,y,carry,&carry);
        return carry;
    }

    constexpr
    auto product (unsigned int x, unsigned int y) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto product_accumulate (unsigned int & x, unsigned int y) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        x = r;
        return r >> 32;
    }

    constexpr
    auto product_sum (unsigned int x, unsigned int y, unsigned int z) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto product_sum_accumulate (unsigned int & x, unsigned int y, unsigned int z) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        x = r;
        return r >> 32;
    }

    constexpr
    auto twice (unsigned int x, unsigned N) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) << N;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto twice_accumulate (unsigned int & x, unsigned N) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) << N;
        x = r;
        return r >> 32;
    }

    constexpr
    auto twice_sum (unsigned int x, unsigned N, unsigned int y) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto twice_sum_accumulate (unsigned int & x, unsigned N, unsigned int y) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        x = r;
        return r >> 32;
    }

    constexpr
    auto half (unsigned int x, unsigned N) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) >> N;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto half_accumulate (unsigned int & x, unsigned N) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) >> N;
        x = r;
        return r >> 32;
    }

    constexpr
    auto half_sum (unsigned int x, unsigned N, unsigned int y) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) >> N ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto half_sum_accumulate (unsigned int & x, unsigned N, unsigned int y) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) >> N ) + y;
        x = r;
        return r >> 32;
    }

    constexpr
    auto square (unsigned int x) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) * x;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto square_accumulate (unsigned int & x) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) * x;
        x = r;
        return r >> 32;
    }

    constexpr
    auto square_sum (unsigned int x, unsigned int y) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    constexpr
    auto square_sum_accumulate (unsigned int & x, unsigned int y) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + y;
        x = r;
        return r >> 32;
    }

    constexpr
    auto ratio (unsigned int x, unsigned int y) noexcept -> array< unsigned int, 2 >
    {
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }

    /// 32-bit fixed point division reciprocal of nonzero divisor.
    auto reciprocal_nonzero ( unsigned y ) -> unsigned
    // requires 0 < y
    {
        assert( 0U < y );
        // 2^32 / y
        return 0xFFFFFFFFu / y;
    }

    /// 32-bit fixed point division reciprocal of "normalised" divisor.
    ///
    /// Reference:
    /// "Improved division by invariant integers",
    /// Niels Möller, Torbjörn Granlund.
    auto reciprocal_normalised (unsigned y) -> unsigned
    // requires 2^31 <= y < 2^32
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        // ( ( ( 2^64 - 1 ) / y ) / 2^32 ) - 2^32
        auto x = static_cast< unsigned long long >( 0xFFFFFFFFU - y ) << 32 | 0xFFFFFFFFU;
        auto q = x / y;
        return q >> 32;
    }
}
