// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic:duo;

import :mono;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::tuple;

/// Bi-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By bi-degree integers we mean integers represented with two words.
///
/// This module partition defines procedures with bi-degree integer operands.

export namespace purple
{
    /// Tests.

    /// Tests if zero.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_zero ( span<Integer const,Degree> x ) noexcept -> bool
    {
        auto r = true;
        r = r && is_zero( x[0] );
        r = r && is_zero( x[1] );
        return r;
    }

    /// Tests if *not* zero.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_zero ( span<Integer const,Degree> x ) noexcept -> bool
    {
        auto r = false;
        r = r || not_zero( x[0] );
        r = r || not_zero( x[1] );
        return r;
    }

    /// Tests if odd.
    ///
    /// In a binary machine, this means the least significant bit is 1.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_odd ( span<Integer const,Degree> x ) noexcept -> bool
    {
        return is_odd( x[0] );
    }

    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_odd ( span<Integer const,Degree> x ) noexcept -> bool
    {
        return not_odd( x[0] );
    }

    /// Tests if normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_normalized ( span<Integer const,Degree> y ) noexcept -> bool
    {
        return is_normalized( y[1] );
    }

    /// Tests if *not* normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 0.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_normalized ( span<Integer const,Degree> y ) noexcept -> bool
    {
        return not_normalized( y[1] );
    }

    /// Tests if smaller.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> bool
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 1;
    }

    /// Tests if *not* smaller.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> bool
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 0;
    }

    /// Tests if greater.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_greater ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 1;
    }

    /// Tests if *not* greater.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_greater ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> bool
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 0;
    }

    /// Tests if equal.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_equal ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> bool
    {
        bool r = true;
        r = r && is_equal( x[0], y[0] );
        r = r && is_equal( x[1], y[1] );
        return r;
    }

    /// Tests if *not* equal.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_equal ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> bool
    {
        bool r = false;
        r = r || not_equal( x[0], y[0] );
        r = r || not_equal( x[1], y[1] );
        return r;
    }

    /// Transform procedures.
    ///
    /// TODO: conceptualize

    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is normalized

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto reciprocal_normalized ( span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto v = reciprocal_normalized( y[1] );
        // p ← ( y[1] * v ) % B
        auto p = y[1] * v;
        // p ← ( p + y[0] ) % B
        p = p + y[0];
        if (p < y[0]) {
            v = v - 1;
            if (p >= y[1]) {
                v = v - 1;
                p = p - y[1];
            }
            // p ← ( p - y[1] ) % B
            p = p - y[1];
        }
        auto t = product( v, y[0] );
        // p ← ( p + t[1] ) % B
        p = p + t[1];
        if ( p < t[1] ) {
            v = v - 1;
            auto tp = array { t[0], p };
            if ( not_smaller( span<Integer const,2>(tp), y ) ) {
                v = v - 1;
            }
        }
        return v;
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto next_assign ( span<Integer,Degree> x ) -> Integer
    {
        auto carry = Integer(0);
        carry = sum_assign( x[0], Integer(1), carry );
        carry = sum_assign( x[1], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_assign ( span<Integer,Degree> x, Integer y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_assign( x[0], y, carry );
        carry = sum_assign( x[1], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_assign ( span<Integer,Degree> x, span<Integer const,Degree> y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_assign( x[0], y[0], carry );
        carry = sum_assign( x[1], y[1], carry );
        return carry;
    }

    /// Twice N times with excess.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto twice_assign ( span<Integer,Degree> x, size_t times = 1 ) noexcept -> Integer
    {
        auto excess = Integer(0);
        excess = twice_assign( x[0], times, excess );
        excess = twice_assign( x[1], times, excess );
        return excess;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring with a "borrow" or leaving a "remainder".

    /// Difference with borrow.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_assign ( span<Integer,Degree> x, Integer y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_assign( x[0], y, borrow );
        borrow = difference_assign( x[1], Integer(0), borrow );
        return borrow;
    }

    /// Difference with borrow.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_assign ( span<Integer,Degree> x, span<Integer const,Degree> y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_assign( x[0], y[0], borrow );
        borrow = difference_assign( x[1], y[1], borrow );
        return borrow;
    }

    /// Half N times with remainder.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto half_assign ( span<Integer,Degree> x, size_t times = 1 ) noexcept -> Integer
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        auto r = x[0] & ((1 << times) - 1);
        x[0] >>= times;
        x[0] |= x[1] << (B - times);
        x[1] >>= times;
        return r;
    }

    /// Normalized division with remainder.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto division_normalized ( span<Integer const,Degree> x, Integer y, Integer iy ) noexcept -> tuple< Integer, Integer >
    {
        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_assign( span(t), x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _ ] = sum( t[1], Integer(1) );
        // r = ( x - ( q × y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _ ] = difference( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            ignore = previous_assign( q );
            ignore = sum_assign( r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            ignore = next_assign( q );
            ignore = difference_assign( r, y );
        }
        // terminate
        return { q, r };
    }
}