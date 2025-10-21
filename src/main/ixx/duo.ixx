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
/// This module partition defines integer arithmetics on bi-degree integers,
/// that is, integers represented with two "limbs".
///
/// These procedures are convenient for the definition of operators on higher-degree integers.
///
/// In the specification of this module partition, let B = 2 ^ bits be the numeric base of a single "limb".
/// A bi-degree value is therefore represented in base B^2.

export namespace purple
{
    /// Tests.
    ///
    /// These procedures return 1 if the test holds, else return 0.

    /// Tests if greater.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_greater ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry;
    }

    /// Tests if normalized.
    ///
    /// Normalized means there are no leading zeros.
    ///
    /// In a binary machine, this means the most significant bit is 1.
    template <typename Integer, size_t Degree>
    constexpr
    auto is_normalized ( span<Integer const,Degree> y ) noexcept -> unsigned
    {
        return is_normalised( y[1] );
    }

    /// Tests if smaller.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry;
    }

    /// Tests if *not* smaller.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        return 1U - is_smaller( x, y );
    }

    /// Transform procedures.
    ///
    /// TODO: conceptualize

    /// Inverse approximation of normalized value.
    ///
    /// Computes y' such that product x × y' approximates quotient x ÷ y.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto inverse_normalized ( span<Integer const,Degree> y ) noexcept -> Integer;
    // requires is_normalized(y)
    // deferred definition; see further down.

    /// Increase procedures.
    ///
    /// These procedures increase values, maybe with a "carry" or an "excess".

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto next_accumulate ( span<Integer,Degree> x ) -> Integer
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], Integer(1), carry );
        carry = sum_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B^2, set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B^2.
    /// Returns carry.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_accumulate ( span<Integer,Degree> x, Integer y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_accumulate( x[0], y, carry );
        carry = sum_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Computes r = x + y + carry.
    /// If r is higher than B^2, set carry ← 1; else carry ← 0.
    ///
    /// Accumulates x ← r % B^2.
    /// Returns carry.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_accumulate ( span<Integer,Degree> x, span<Integer const,Degree> y, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = sum_accumulate( x[0], y[0], carry );
        carry = sum_accumulate( x[1], y[1], carry );
        return carry;
    }

    /// Twice N times.
    ///
    /// In a binary machine, twice means shifting bits towards most significant.
    ///
    /// Computes r = x × 2 ^ N.
    ///
    /// Accumulates x ← r % B^2.
    /// Returns r ÷ B^2.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto twice_accumulate ( span<Integer,Degree> x, size_t N, Integer carry = Integer(0) ) noexcept -> Integer
    {
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    /// Decrease procedures.
    ///
    /// These procedures decrease values, maybe with a "borrow" or a "remainder".

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_accumulate ( span<Integer,Degree> x, Integer y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_accumulate( x[0], y, borrow );
        borrow = difference_accumulate( x[1], Integer(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Computes r = x - y - borrow.
    /// If r is lower than zero, borrow ← 1; else borrow ← 0.
    ///
    /// Accumulates x ← |r|.
    /// Returns borrow.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_accumulate ( span<Integer,Degree> x, span<Integer const,Degree> y, Integer borrow = Integer(0) ) noexcept -> Integer
    {
        borrow = difference_accumulate( x[0], y[0], borrow );
        borrow = difference_accumulate( x[1], y[1], borrow );
        return borrow;
    }

    /// Half N times with remainder.
    ///
    /// In a binary machine, half means shifting bits towards least significant.
    ///
    /// Computes q = x ÷ 2 ^ N and r = x % 2 ^ N.
    ///
    /// Accumulates x ← q.
    /// Returns r.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto half_accumulate ( span<Integer,Degree> x, size_t N ) noexcept -> Integer
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        auto r = x[0] & ((1 << N) - 1);
        x[0] >>= N;
        x[0] |= x[1] << (B - N);
        x[1] >>= N;
        return r;
    }

    /// Division, quotient and remainder, with normalized operands.
    ///
    /// Computes q = x ÷ y and r = x % y.
    ///
    /// Computes by the "improved division by invariant integer" method.
    ///
    /// Returns { q, r }.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto division_normalized ( span<Integer const,Degree> x, Integer y, Integer iy ) noexcept -> tuple< Integer, Integer >
    // requires is_smaller( x[1], y )
    // requires is_normalized(y)
    // requires iy = inverse_normalized(y)
    {
        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_accumulate( span(t), x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _0 ] = sum( t[1], Integer(1) );
        // r = ( x - ( q × y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _1 ] = difference( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            ignore = difference_accumulate( q, Integer(1) );
            ignore = sum_accumulate( r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            ignore = sum_accumulate( q, Integer(1) );
            ignore = difference_accumulate( r, y );
        }
        // terminate
        return { q, r };
    }

    /// Division, quotient and remainder.
    ///
    /// Computes q = x ÷ y and r = x % y.
    ///
    /// Computes by the "improved division by invariant integer" method.
    ///
    /// Returns { q, r }.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto division ( span<Integer const,Degree> x, Integer y ) noexcept -> tuple< array<Integer,2>, Integer >
    // requires not_zero(y)
    {
        auto q = array { Integer(0), Integer(0) };
        auto r = array { x[0], x[1] };

        // normalize dividend
        if ( r[1] >= y )
            tie( q[1], r[1] ) = division( r[1], y ); // TODO: ...with inverse
        // invariant: r[1] < y

        // normalize divisor and remainder
        auto ylz = top_zeros( y );
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate( span(r), ylz );
        // invariant: y is normalized

        // compute divisor inverse
        auto iy = inverse_normalized( y );

        // compute division with normalized operands
        tie( q[0], r[0] ) = division_normalized( span<Integer const,2>(r), y, iy );

        // denormalize remainder
        half_accumulate( r[0], ylz );

        // terminate
        return { q, r[0] };
    }
}

// Non-const to const conversions.

export namespace purple
{
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto is_greater ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        auto xx = span<Integer const,Degree>(x);
        auto yy = span<Integer const,Degree>(y);
        return is_greater<Integer,Degree>( xx, yy );
    }

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<Integer const,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<Integer,Degree> x, span<Integer const,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( span<Integer const,Degree>(x), y );
    }

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( span<Integer const,Degree>(x), span<Integer const,Degree>(y) );
    }

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_accumulate ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return sum_accumulate<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_accumulate ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return difference_accumulate<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }
}

// Deferred definitions.

namespace purple
{
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto inverse_normalized ( span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto v = inverse_normalized( y[1] );
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
}