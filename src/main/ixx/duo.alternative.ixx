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

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( array<Word,Degree> const & x, array<Word,Degree> const & y ) noexcept -> Word
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
    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto division_v0 ( span<Word const,Degree> x, Word y ) noexcept -> tuple< array<Word,Degree>, Word >
    // requires not_zero(y)
    {
        // let q ← 0
        auto q = array { Word(0), Word(0) };
        // let r ← x
        auto r = array { x[0], x[1] };
        while ( not_smaller<unsigned,2>( r, array { y, Word(0) } ) ) { // TODO: not_smaller 2D vs 1D
            // q ← q + 1
            ignore = next_assign<Word,Degree>( q, q );
            // r ← r - y
            ignore = difference_assign<Word,Degree>( r, r, y );
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
    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto division_v1 ( span<Word const,Degree> x, Word y ) noexcept -> tuple< array<Word,Degree>, Word >
    // requires not_zero(y)
    {
        // reinterpret dividend
        constexpr auto Bits = sizeof(Word) * 8uz;
        using DoubleWord = _BitInt( Bits * 2 );
        auto x_ = ( DoubleWord( x[1] ) << Bits ) | x[0];
        // lookup answer with the "tool"
        auto q = x_ / y;
        auto r = x_ % y;
        // terminate
        auto q0 = Word( q );
        auto q1 = Word( q >> Bits );
        return { array { q0, q1 }, r };
    }

    /// Quotient and remainder by "normalised" divisor with inverse.
    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto ratio_normalised_v0 ( span<Word,Degree> x, Word y, Word iy ) -> tuple< Word, Word >
    // requires B/2 <= y < B
    // requires x[1] < y
    // requires iy = ( (B^2 - 1) / y ) - B
    {
        auto [q,q0] = product( x[1], iy );
        ignore = sum_assign( q, x[1] );
        auto pp = product( q, y );
        auto r = array<unsigned,2> { x[0], x[1] };
        ignore = difference_assign<Word>( r, pp );
        while ( is_greater( r[1], Word(0) ) || not_smaller( r[0], y ) ) {
            ignore = sum_assign( q, 1 );
            ignore = difference_assign( r, y );
        }
        return { q, r[0] };
    }
}
