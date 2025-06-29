// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:duo.alternative;

import :mono;
import :duo;

using std::array;
using std::ignore;
using std::span;
using std::tie;
using std::tuple;

// Duo-degree arithmetic.

export namespace purple
{
    /// Accumulate remainder, return quotient, by "normalised" divisor with inverse.
    template <typename Integer>
    auto ratio_normalised_accumulate_v0 ( span<Integer,2> r, Integer y, Integer iy ) -> Integer
    // requires B/2 <= y < B
    // requires r[1] < y
    // requires iy = ( (B^2 - 1) / y ) - B
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( r[1] < y );
        assert( iy == inverse_normalised(y) );

        auto [q,q0] = product( r[1], iy );
        ignore = sum_accumulate( q, r[1] );
        auto pp = product( q, y );
        ignore = difference_accumulate<Integer>( r, pp );
        while ( is_greater( r[1], Integer(0) ) || not_smaller( r[0], y ) ) {
            ignore = sum_accumulate( q, 1 );
            ignore = difference_accumulate( r, y );
        }
        return q;
    }
}
