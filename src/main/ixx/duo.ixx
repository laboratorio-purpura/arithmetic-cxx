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

export namespace purple
{
    // Accumulate minus and return carry.
    template <typename Integer>
    auto minus_accumulate ( span<Integer,2> x, Integer y )
    {
        auto carry = Integer(0);
        carry = minus_accumulate( x[0], y, carry );
        carry = minus_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    // Accumulate minus and return carry.
    template <typename Integer>
    auto minus_accumulate ( span<Integer,2> x, span<Integer,2> y )
    {
        auto carry = Integer(0);
        carry = minus_accumulate( x[0], y[0], carry );
        carry = minus_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate twice and return carry.
    template <typename Integer>
    auto twice_accumulate ( span<Integer,2> x, unsigned N )
    {
        auto carry = Integer(0);
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    // Accumulate half rounded down.
    template <typename Integer>
    auto half_accumulate ( span<Integer,2> r, unsigned N ) noexcept
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        r[0] >>= N;
        r[0] |= r[1] << (B - N);
        r[1] >>= N;
    }

    /// Accumulate remainder and return singular quotient by "normalised" divisor & reciprocal.
    template <typename Integer>
    auto ratio_singular_normalised_accumulate ( span<Integer,2> r, Integer y, Integer y_ ) -> Integer
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( r[1] < y );
        assert( y_ == reciprocal_normalised(y) );

        auto [q,q0] = product( r[1], y_ );
        ignore = sum_accumulate( q, r[1] );
        auto pp = product( q, y );
        ignore = minus_accumulate<Integer>( r, pp );
        while ( is_greater( r[1], Integer(0) ) || not_smaller( r[0], y ) ) {
            ignore = sum_accumulate( q, 1 );
            ignore = minus_accumulate( r, y );
        }
        return q;
    }

    /// Quotient and remainder.
    template <typename Integer>
    auto ratio ( span<Integer,2> x, Integer y ) -> tuple< array<Integer,2>, Integer >
    {
        assert( 0 < y );

        auto q = array<Integer,2> { };
        auto r = array<Integer,2> { x[0], x[1] };

        if ( r[1] >= y )
            tie( q[1], r[1] ) = ratio( r[1], y ); // TODO: ...with reciprocal
        assert( r[1] < y );

        auto ylz = __builtin_clzg( y ); // TODO: generalize
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate<Integer>( r, ylz );
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );

        auto y_ = purple::reciprocal_normalised( y );

        q[0] = ratio_singular_normalised_accumulate<Integer>( r, y, y_ );

        half_accumulate<Integer>( r, ylz );

        return { q, r[0] };
    }
}
