// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>
#include <tuple>

export module purple.arithmetic:reciprocal;

import :word_concept;

export namespace purple
{
    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Computes by the "improved division by invariant integers" method.
    ///
    /// Requires:
    /// y is normalized

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto reciprocal_normalized ( span<W const,Bi> y ) noexcept -> W
    {
        auto v = reciprocal_normalized( y[1] );
        // p ← ( y[1] * v ) % B
        auto [ p, _ ] = product ( y[1], v );
        // p ← ( p + y[0] ) % B
        tie( p, ignore ) = sum( p, y[0] );
        if ( is_smaller( p, y[0] ) ) {
            tie( v, ignore ) = previous( v );
            if ( not_smaller( p, y[1] ) ) {
                tie( v, ignore ) = previous( v );
                tie( p, ignore ) = difference( p, y[1] );
            }
            // p ← ( p - y[1] ) % B
            tie( p, ignore ) = difference( p, y[1] );
        }
        auto t = product( v, y[0] );
        // p ← ( p + t[1] ) % B
        tie( p, ignore ) = sum( p, t[1] );
        if ( is_smaller( p, t[1] ) ) {
            tie( v, ignore ) = previous( v );
            auto tp = array { t[0], p };
            if ( not_smaller<W>( tp, y ) ) {
                tie( v, ignore ) = previous( v );
            }
        }
        return v;
    }
}
