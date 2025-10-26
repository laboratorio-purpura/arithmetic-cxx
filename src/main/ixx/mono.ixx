// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <tuple>

export module purple.arithmetic:mono;

using std::array;
using std::size_t;
using std::tuple;

/// Singular nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary computer, B = 2 ^ bits.
///
/// By singular integers we mean integers represented with a single word.
///
/// This module partition defines procedures with singular integer operands.

export namespace purple
{
    static_assert( sizeof(unsigned) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// Test procedures.

    /// Tests if zero.

    constexpr
    auto is_zero ( unsigned x ) noexcept -> bool
    {
        return x == 0u;
    }

    /// Tests if *not* zero.

    constexpr
    auto not_zero ( unsigned x ) noexcept -> bool
    {
        return x != 0u;
    }

    /// Tests if odd.
    ///
    /// In a binary machine, this means the least significant bit is 1.

    constexpr
    auto is_odd ( unsigned x ) noexcept -> bool
    {
        return (x & 1) == 1;
    }

    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    constexpr
    auto not_odd ( unsigned x ) noexcept -> bool
    {
        return (x & 1) == 0;
    }

    /// Tests if normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    constexpr
    auto is_normalized ( unsigned y ) noexcept -> bool
    {
        return (y >> 31uz) == 1;
    }

    /// Tests if *not* normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y < B.
    ///
    /// In a binary machine, this means the most significant bit is 0.

    constexpr
    auto not_normalized ( unsigned y ) noexcept -> bool
    {
        return (y >> 31uz) == 0;
    }

    /// Tests if smaller.

    constexpr
    auto is_smaller ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x < y;
    }

    /// Tests if *not* smaller.

    constexpr
    auto not_smaller ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x >= y;
    }

    /// Tests if greater.

    constexpr
    auto is_greater ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x > y;
    }

    /// Tests if *not* greater.

    constexpr
    auto not_greater ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x <= y;
    }

    /// Tests if equal.

    constexpr
    auto is_equal ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x == y;
    }

    /// Tests if *not* equal

    constexpr
    auto not_equal ( unsigned x, unsigned y ) noexcept -> bool
    {
        return x != y;
    }

    /// Query procedures.

    /// Count of leading zeros.
    ///
    /// Requires:
    /// x is nonzero
    ///
    /// In a binary machine, this means N ⇒ 2^N ≤ x < 2^N+1

    constexpr
    auto leading_zero_bits ( unsigned x ) noexcept -> size_t
    {
        return __builtin_clzg( x );
    }

    /// Transform procedures.

    /// Reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is nonzero

    constexpr
    auto reciprocal ( unsigned y ) noexcept -> unsigned
    {
        // ( B - 1 ) ÷ y
        return 0xFFFFFFFFu / y;
    }

    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is normalized

    constexpr
    auto reciprocal_normalized ( unsigned y ) noexcept -> unsigned
    {
        // ( ( ( B^2 - 1 ) ÷ y ) ÷ B ) - B
        auto x = static_cast< unsigned long long >( 0xFFFFFFFFu - y ) << 32 | 0xFFFFFFFFu;
        auto q = x / y;
        return q;
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.

    constexpr
    auto next ( unsigned x, unsigned carry = 0 ) noexcept -> array< unsigned, 2uz >
    {
        auto r = __builtin_addc(x,1u,carry,&carry);
        return { r, carry };
    }

    /// Next with carry.

    constexpr
    auto next_assign ( unsigned & x, unsigned carry = 0 ) noexcept -> unsigned
    {
        x = __builtin_addc(x,1u,carry,&carry);
        return carry;
    }

    /// Sum with carry.

    constexpr
    auto sum ( unsigned x, unsigned y, unsigned carry = 0 ) noexcept -> array< unsigned, 2uz >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    /// Sum with carry.

    constexpr
    auto sum_assign ( unsigned & x, unsigned y, unsigned carry = 0 ) noexcept -> unsigned
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    /// Product with excess.

    constexpr
    auto product ( unsigned x, unsigned y, unsigned excess = 0 ) noexcept -> array< unsigned, 2uz >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + excess;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Product with excess.

    constexpr
    auto product_assign ( unsigned & x, unsigned y, unsigned excess = 0 ) noexcept -> unsigned
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + excess;
        x = r;
        return r >> 32;
    }

    /// Twice with excess.

    constexpr
    auto twice ( unsigned x, size_t z, unsigned excess = 0 ) noexcept -> array< unsigned, 2uz >
    {
        auto r = ( static_cast<unsigned long long>(x) << z ) + excess;
        return {
            static_cast<unsigned>( r ),
            static_cast<unsigned>( r >> 32 )
        };
    }

    /// Twice with excess.

    constexpr
    auto twice_assign ( unsigned & x, size_t z, unsigned excess = 0 ) noexcept -> unsigned
    {
        auto r = ( static_cast<unsigned long long>(x) << z ) + excess;
        x = r;
        return r >> 32;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.

    constexpr
    auto previous ( unsigned x, unsigned borrow = 0 ) noexcept -> tuple< unsigned, unsigned >
    {
        auto r = __builtin_subc(x,1u,borrow,&borrow);
        return { r, borrow };
    }

    /// Previous with borrow.

    constexpr
    auto previous_assign ( unsigned & x, unsigned borrow = 0 ) noexcept -> unsigned
    {
        x = __builtin_subc(x,1u,borrow,&borrow);
        return borrow;
    }

    /// Difference with borrow.

    constexpr
    auto difference ( unsigned x, unsigned y, unsigned borrow = 0 ) noexcept -> tuple< unsigned, unsigned >
    {
        auto r = __builtin_subc(x,y,borrow,&borrow);
        return { r, borrow };
    }

    /// Difference with borrow.

    constexpr
    auto difference_assign ( unsigned & x, unsigned y, unsigned borrow = 0 ) noexcept -> unsigned
    {
        x = __builtin_subc(x,y,borrow,&borrow);
        return borrow;
    }

    /// Division with remainder.
    ///
    /// Requires:
    /// y is nonzero

    constexpr
    auto division ( unsigned x, unsigned y ) noexcept -> tuple< unsigned, unsigned >
    {
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }

    /// Division with remainder.
    ///
    /// Requires:
    /// y is nonzero

    constexpr
    auto division_assign ( unsigned & x, unsigned y ) noexcept -> unsigned
    {
        auto q = x / y;
        auto r = x % y;
        x = q;
        return r;
    }

    /// Half with remainder.

    constexpr
    auto half ( unsigned x, size_t z ) noexcept -> tuple< unsigned, unsigned >
    {
        auto q = x >> z;
        auto r = x & ((1 << z) - 1);
        return { q, r };
    }

    /// Half with remainder.

    constexpr
    auto half_assign ( unsigned & x, size_t z ) -> unsigned
    {
        auto q = x >> z;
        auto r = x & ((1 << z) - 1);
        x = q;
        return r;
    }
}