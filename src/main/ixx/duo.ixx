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

// Core interfaces.

export namespace purple
{
    // Accumulate minus and return carry.
    auto minus_accumulate ( span<unsigned,2> x, unsigned y )
    {
        unsigned carry { 0 };
        carry = minus_accumulate( x[0], y, carry );
        carry = minus_accumulate( x[1], 0U, carry );
        return carry;
    }

    // Accumulate minus and return carry.
    auto minus_accumulate ( span<unsigned,2> x, span<unsigned,2> y )
    {
        unsigned carry { 0 };
        carry = minus_accumulate( x[0], y[0], carry );
        carry = minus_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate twice and return carry.
    auto twice_accumulate ( span<unsigned,2> x, unsigned N )
    {
        unsigned carry { 0 };
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    // Accumulate half rounded down.
    auto half_accumulate (span<unsigned,2> r, unsigned N) noexcept
    {
        constexpr auto B = sizeof(unsigned) * 8uz;
        r[0] >>= N;
        r[0] |= r[1] << (B - N);
        r[1] >>= N;
    }

    /// Accumulate remainder and return singular quotient by "normalised" divisor & reciprocal.
    auto ratio_singular_normalised_accumulate ( span<unsigned,2> r, unsigned y, unsigned y_ ) -> unsigned
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( r[1] < y );
        assert( y_ == reciprocal_normalised(y) );

        auto [q,q0] = product( r[1], y_ );
        ignore = sum_accumulate( q, r[1] );
        auto pp = product( q, y );
        ignore = minus_accumulate( r, pp );
        while ( r[1] > 0U || r[0] >= y ) {
            q += 1;
            ignore = minus_accumulate( r, y );
        }
        return q;
    }
}

// Convenient interfaces.

export namespace purple
{
    /// Quotient and remainder.
    auto ratio ( span<unsigned,2> x, unsigned y ) -> tuple< array<unsigned,2>, unsigned >
    {
        assert( 0 < y );

        array<unsigned,2> q {};
        array<unsigned,2> r { x[0], x[1] };

        if ( r[1] >= y )
            tie( q[1], r[1] ) = ratio( r[1], y ); // TODO: ...with reciprocal
        assert( r[1] < y );

        auto ylz = __builtin_clzg( y );
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate( r, ylz );
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );

        auto y_ = purple::reciprocal_normalised( y );

        q[0] = ratio_singular_normalised_accumulate( r, y, y_ );

        half_accumulate( r, ylz );

        return { q, r[0] };
    }
}
