// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <stdint.h>
#include <tuple>

export module purple.arithmetic:mono;

using std::array;
using std::size_t;
using std::tuple;

/// Singular nonnegative integer arithmetics.
///
/// This module partition defines nonnegative integer arithmetics on singular integers,
/// that is, integers represented with a single numeric "limb".
///
/// Machines usually offer primitive operators for singular nonnegative arithmetics with carry or borrow.
/// Programming languages usually expose these primitive operators but omitting the carry or borrow.
/// This module partition defines procedures for singular nonnegative arithmetics with carry or borrow.
///
/// Many additional procedures are defined with the purpose of obtaining a basic "arithmetic language",
/// so that procedures on higher-degree integers may be expressed completely in this basic language.
///
/// In the specification of this module partition, let B = 2 ^ bits be the numeric base of a single "limb".

export namespace purple
{
    static_assert( sizeof(unsigned int) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// Test procedures.
    ///
    /// These procedures return 1 if the test holds, else return 0.

    /// Tests if normalized.
    ///
    /// Normalized means there are no leading zeros.
    ///
    /// In a binary machine, this means the most significant bit is 1.
    constexpr
    auto is_normalized ( unsigned y ) noexcept -> unsigned
    {
        return y >> 31u;
    }

    /// Tests if zero.
    constexpr
    auto is_zero ( unsigned int x ) noexcept -> unsigned int
    {
        x |= -x;
        x >>= 31u;
        return 1u - x;
    }

    /// Tests if *not* zero.
    constexpr
    auto not_zero ( unsigned int x ) noexcept -> unsigned int
    {
        x |= -x;
        return x >> 31u;
    }

    /// Tests if smaller.
    constexpr
    auto is_smaller ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        auto r0 = x ^ y;
        auto r1 = x - y;
        r1 ^= r0 & (r1 ^ x ^ (1u << 31));
        return r1 >> 31;
    }

    /// Tests if *not* smaller.
    constexpr
    auto not_smaller ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1u - is_smaller( x, y );
    }

    /// Tests if greater.
    constexpr
    auto is_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return is_smaller( y, x );
    }

    /// Tests if *not* greater.
    constexpr
    auto not_greater ( unsigned int x, unsigned int y ) noexcept -> unsigned int
    {
        return 1u - is_smaller( y, x );
    }

    /// Transform procedures.
    ///
    /// TODO: conceptualize

    /// Inverse approximation.
    ///
    /// Computes y' such that product x × y' approximates quotient x ÷ y.
    constexpr
    auto inverse ( unsigned y ) noexcept -> unsigned
    // requires not_zero(y)
    {
        // ( B - 1 ) ÷ y
        return 0xFFFFFFFFu / y;
    }

    /// Inverse approximation of normalized integer.
    ///
    /// Computes y' such that product x × y' approximates quotient x ÷ y.
    constexpr
    auto inverse_normalized ( unsigned y ) noexcept -> unsigned
    // requires is_normalized(y)
    {
        // ( ( ( B^2 - 1 ) ÷ y ) ÷ B ) - B
        auto x = static_cast< unsigned long long >( 0xFFFFFFFFu - y ) << 32 | 0xFFFFFFFFu;
        auto q = x / y;
        return q;
    }

    /// Counts leading zeros in representation.
    ///
    /// TODO: explain what this means, perhaps rename
    constexpr
    auto top_zeros ( unsigned x ) noexcept
    // requires not_zero(x)
    {
        return __builtin_clzg( x );
    }

    /// Increase procedures.
    ///
    /// These procedures increase values, maybe with a "carry" or an "excess".

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B, set carry ← 1; else carry ← 0.
    ///
    /// Returns { r % B, carry }.
    constexpr
    auto sum ( unsigned int x, unsigned int y, unsigned int carry = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B, set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B.
    /// Returns carry.
    constexpr
    auto sum_accumulate ( unsigned int & x, unsigned int y, unsigned int carry = 0 ) noexcept -> unsigned int
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    /// Twice N times.
    ///
    /// In a binary machine, twice means shifting bits towards most significant.
    ///
    /// Computes r = x × 2 ^ N.
    ///
    /// Returns { r % B, r ÷ B }.
    constexpr
    auto twice ( unsigned int x, size_t N ) noexcept -> array< unsigned int, 2 >
    {
        auto r = static_cast<unsigned long long>(x) << N;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Twice N times.
    ///
    /// In a binary machine, twice means shifting bits towards most significant.
    ///
    /// Computes r = x × 2 ^ N.
    ///
    /// Accumulates x ← r % B.
    /// Returns r ÷ B.
    constexpr
    auto twice_accumulate ( unsigned int & x, size_t N ) noexcept -> unsigned int
    {
        auto r = static_cast<unsigned long long>(x) << N;
        x = r;
        return r >> 32;
    }

    /// Twice N times and sum.
    ///
    /// Computes r = ( x × 2 ^ N ) + y.
    ///
    /// Returns { r % B, r ÷ B }.
    constexpr
    auto twice_sum ( unsigned int x, size_t N, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Twice N times and sum.
    ///
    /// Computes r = ( x × 2 ^ N ) + y.
    ///
    /// Accumulates x ← r % B.
    /// Returns r ÷ B.
    constexpr
    auto twice_sum_accumulate ( unsigned int & x, size_t N, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) << N ) + y;
        x = r;
        return r >> 32;
    }

    /// Product.
    ///
    /// Computes r = x × y.
    ///
    /// Returns { r % B, r ÷ B }.
    constexpr
    auto product ( unsigned int x, unsigned int y ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Product.
    ///
    /// Computes r = x × y.
    ///
    /// Accumulates x ← r % B.
    /// Returns r ÷ B.
    constexpr
    auto product_accumulate ( unsigned int & x, unsigned int y ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y );
        x = r;
        return r >> 32;
    }

    /// Product and sum.
    ///
    /// Computes r = x × y + z.
    ///
    /// Returns { r % B, r ÷ B }.
    constexpr
    auto product_sum ( unsigned int x, unsigned int y, unsigned int z ) noexcept -> array< unsigned int, 2 >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Product and sum.
    ///
    /// Computes r = x × y + z.
    ///
    /// Accumulates x ← r % B.
    /// Returns r ÷ B.
    constexpr
    auto product_sum_accumulate ( unsigned int & x, unsigned int y, unsigned int z ) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + z;
        x = r;
        return r >> 32;
    }

    /// Decrease procedures.
    ///
    /// These procedures decrease values, maybe with a "borrow" or a "remainder".

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Returns { |r|, borrow }.
    constexpr
    auto difference ( unsigned int x, unsigned int y, unsigned int borrow = 0 ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_subc(x,y,borrow,&borrow);
        return { r, borrow };
    }

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    constexpr
    auto difference_accumulate ( unsigned int & x, unsigned int y, unsigned int borrow = 0 ) noexcept -> unsigned int
    {
        x = __builtin_subc(x,y,borrow,&borrow);
        return borrow;
    }

    /// Half N times with remainder.
    ///
    /// In a binary machine, half means shifting bits towards least significant.
    ///
    /// Computes q = x ÷ 2 ^ N, r = x % 2 ^ N.
    ///
    /// Returns { q, r }.
    constexpr
    auto half ( unsigned int x, size_t N ) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = x & ((1 << N) - 1);
        auto q = x >> N;
        return { q, r };
    }

    /// Half N times with remainder.
    ///
    /// In a binary machine, half means shifting bits towards least significant.
    ///
    /// Computes q = x ÷ 2 ^ N, r = x % 2 ^ N.
    ///
    /// Accumulates x ← q.
    /// Returns r.
    constexpr
    auto half_accumulate ( unsigned int & x, size_t N ) -> unsigned int
    {
        auto [ q, r ] = half(x,N);
        x = q;
        return r;
    }

    /// Division, quotient and remainder.
    ///
    /// Computes q = x ÷ y, r = x % y.
    ///
    /// Returns { q, r }.
    constexpr
    auto division ( unsigned int x, unsigned int y ) noexcept -> tuple< unsigned int, unsigned int >
    // requires not_zero(y)
    {
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }
}
