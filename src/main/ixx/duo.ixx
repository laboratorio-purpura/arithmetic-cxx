// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:duo;

import :mono;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;

// Duo-degree arithmetic.

export namespace purple
{
    // Accumulate sum, return carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer,2> x, Integer y )
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], y, carry );
        carry = sum_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    // Accumulate sum, return carry.
    template <typename Integer>
    auto sum_accumulate ( span<Integer,2> x, span<Integer,2> y )
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], y[0], carry );
        carry = sum_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate difference, return carry.
    template <typename Integer>
    auto difference_accumulate ( span<Integer,2> x, Integer y )
    {
        auto carry = Integer(0);
        carry = difference_accumulate( x[0], y, carry );
        carry = difference_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    // Accumulate difference, return carry.
    template <typename Integer>
    auto difference_accumulate ( span<Integer,2> x, span<Integer,2> y )
    {
        auto carry = Integer(0);
        carry = difference_accumulate( x[0], y[0], carry );
        carry = difference_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate twice N times, return carry.
    template <typename Integer>
    auto twice_accumulate ( span<Integer,2> x, size_t N )
    {
        auto carry = Integer(0);
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    // Accumulate half N times, rounded down.
    template <typename Integer>
    auto half_accumulate ( span<Integer,2> r, size_t N ) noexcept
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        r[0] >>= N;
        r[0] |= r[1] << (B - N);
        r[1] >>= N;
    }

    /// Quotient and remainder by "normalised" divisor with inverse.
    template <typename Integer>
    auto ratio_normalised ( span<Integer,2> x, Integer y, Integer iy ) -> tuple< Integer, Integer >
    // requires B/2 <= y < B
    // requires x[1] < y
    // requires iy = ( (B^2 - 1) / y ) - B
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( x[1] < y );
        assert( iy == inverse_normalised(y) );

        // t = ( r[1] * iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_accumulate<Integer>( t, x );
        // q = ( t[1] + 1 ) mod B
        auto q = sum_modulus( t[1], Integer(1) );
        // r = ( x - ( q * y ) ) mod B
        auto qy = product( q, y );
        auto r = difference_modulus( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            q = difference_modulus( q, 1 );
            r = sum_modulus( r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            q = sum_modulus( q, 1 );
            r = difference_modulus( r, y );
        }
        // terminate
        return { q, r };
    }

    /// Quotient and remainder.
    template <typename Integer>
    auto ratio ( span<Integer,2> x, Integer y ) -> tuple< array<Integer,2>, Integer >
    // requires y != 0
    {
        assert( 0 < y );

        auto q = array<Integer,2> { Integer(0), Integer(0) };
        auto r = array<Integer,2> { x[0], x[1] };

        // "normalise" dividend
        if ( r[1] >= y )
            tie( q[1], r[1] ) = ratio( r[1], y ); // TODO: ...with inverse
        assert( r[1] < y );

        // "normalise" divisor and remainder
        auto ylz = top_zeros( y );
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate<Integer>( r, ylz );
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );

        // compute dividend inverse
        auto iy = inverse_normalised( y );

        // compute normalised ratio
        tie( q[0], r[0] ) = ratio_normalised<Integer>( r, y, iy );

        // "denormalise" remainder
        half_accumulate<Integer>( r, ylz );

        // terminate
        return { q, r[0] };
    }
}
