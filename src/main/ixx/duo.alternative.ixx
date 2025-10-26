// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic.alternative:duo;

import purple.arithmetic;

using std::array;
using std::ignore;
using std::span;
using std::tuple;

/// Bi-degree nonnegative integer arithmetics.

/// Type conversions.

namespace purple
{
    /// Arrays of non-const.

    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( array<Integer,Degree> const & x, array<Integer,Degree> const & y ) noexcept -> Integer
    {
        auto xx = span(x);
        auto yy = span(y);
        return not_smaller( xx, yy );
    }
}

export namespace purple
{
    /// Reduction operators.

    /// Division, quotient and remainder.
    ///
    /// Computes q = x ÷ y and r = x % y.
    ///
    /// Computes by definition: successive subtractions.
    ///
    /// Returns { q, r }.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto division_v0 ( span<Integer const,Degree> x, Integer y ) noexcept -> tuple< array<Integer,Degree>, Integer >
    // requires not_zero(y)
    {
        // let q ← 0
        auto q = array { Integer(0), Integer(0) };
        // let r ← x
        auto r = array { x[0], x[1] };
        while ( not_smaller<unsigned,2>( r, array { y, Integer(0) } ) ) { // TODO: not_smaller 2D vs 1D
            // q ← q + 1
            ignore = next_assign<Integer,Degree>( q );
            // r ← r - y
            ignore = difference_assign<Integer,Degree>( r, y );
        }
        // terminate
        return { q, r [0] };
    }

    /// Division, quotient and remainder.
    ///
    /// Computes q = x ÷ y and r = x % y.
    ///
    /// Computes by "tool".
    /// Assumes we have a tool capable of solving base B division of two-degree dividends by one-degree divisors.
    /// In the past, this might have been a division table, or a sliding rule.
    /// Our current tool is, of course, the binary electronic computer processor.
    ///
    /// Returns { q, r }.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto division_v1 ( span<Integer const,Degree> x, Integer y ) noexcept -> tuple< array<Integer,Degree>, Integer >
    // requires not_zero(y)
    {
        // reinterpret dividend
        constexpr auto Bits = sizeof(Integer) * 8uz;
        using DoubleInteger = _BitInt( Bits * 2 );
        auto x_ = ( DoubleInteger( x[1] ) << Bits ) | x[0];
        // lookup answer with the "tool"
        auto q = x_ / y;
        auto r = x_ % y;
        // terminate
        auto q0 = Integer( q );
        auto q1 = Integer( q >> Bits );
        return { array { q0, q1 }, r };
    }

    /// Quotient and remainder by "normalised" divisor with inverse.
    template <typename Integer, size_t Degree>
    requires ( Degree == 2uz )
    auto ratio_normalised_v0 ( span<Integer,Degree> x, Integer y, Integer iy ) -> tuple< Integer, Integer >
    // requires B/2 <= y < B
    // requires x[1] < y
    // requires iy = ( (B^2 - 1) / y ) - B
    {
        auto [q,q0] = product( x[1], iy );
        ignore = sum_assign( q, x[1] );
        auto pp = product( q, y );
        auto r = array<unsigned,2> { x[0], x[1] };
        ignore = difference_assign<Integer>( r, pp );
        while ( is_greater( r[1], Integer(0) ) || not_smaller( r[0], y ) ) {
            ignore = sum_assign( q, 1 );
            ignore = difference_assign( r, y );
        }
        return { q, r[0] };
    }
}
