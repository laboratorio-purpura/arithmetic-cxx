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
using std::span;
using std::tie;
using std::tuple;

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

    // Accumulate twice N times and return carry.
    template <typename Integer>
    auto twice_accumulate ( span<Integer,2> x, unsigned N )
    {
        auto carry = Integer(0);
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    // Accumulate half N times rounded down.
    template <typename Integer>
    auto half_accumulate ( span<Integer,2> r, unsigned N ) noexcept
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        r[0] >>= N;
        r[0] |= r[1] << (B - N);
        r[1] >>= N;
    }

    /// Accumulate remainder and return quotient by "normalised" divisor with inverse.
    template <typename Integer>
    auto ratio_normalised_accumulate_v0 ( span<Integer,2> r, Integer y, Integer iy ) -> Integer
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

    /// Accumulate remainder and return quotient by "normalised" divisor with inverse.
    template <typename Integer>
    auto ratio_normalised_accumulate_v1 ( span<Integer,2> r, Integer y, Integer iy ) -> Integer
    // requires 2^31 <= y < 2^32
    // requires r[1] < y
    // requires iy ~ 1 / y
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( r[1] < y );
        assert( iy == inverse_normalised(y) );

        // t = ( r[1] * iy ) + x
        auto t = product( r[1], iy );
        ignore = sum_accumulate<Integer>( t, r );
        // q = ( t[1] + 1 ) mod B
        auto q = sum_modulus( t[1], Integer(1) );
        // r = ( x - ( q * y ) ) mod B
        auto qy = product( q, y );
        r[0] = difference_modulus( r[0], qy[0] );
        r[1] = Integer(0);
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r[0], t[0] ) ) {
            q = difference_modulus( q, 1 );
            r[0] = sum_modulus( r[0], y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r[0], y ) ) [[unlikely]] {
            q = sum_modulus( q, 1 );
            r[0] = difference_modulus( r[0], y );
        }
        // terminate
        return q;
    }

    /// Quotient and remainder.
    template <typename Integer>
    auto ratio ( span<Integer,2> x, Integer y ) -> tuple< array<Integer,2>, Integer >
    {
        assert( 0 < y );

        auto q = array<Integer,2> { Integer(0), Integer(0) };
        auto r = array<Integer,2> { x[0], x[1] };

        if ( r[1] >= y )
            tie( q[1], r[1] ) = ratio( r[1], y ); // TODO: ...with inverse
        assert( r[1] < y );

        auto ylz = top_zeros( y );
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate<Integer>( r, ylz );
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );

        auto iy = inverse_normalised( y );

        q[0] = ratio_normalised_accumulate_v1<Integer>( r, y, iy );

        half_accumulate<Integer>( r, ylz );

        return { q, r[0] };
    }
}
