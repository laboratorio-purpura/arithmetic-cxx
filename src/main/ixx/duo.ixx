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
using std::span;
using std::tuple;

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

    /// Accumulate remainder and return singular quotient by "normalised" divisor with reciprocal.
    auto ratio_singular_normalised_accumulate ( span<unsigned,2> r, unsigned y, unsigned y_ ) -> unsigned
    // requires 2^31 <= y < 2^32
    // requires x[1] < y
    // requires y_ == reciprocal_normalised(y)
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( r[1] < y );
        assert( y_ == reciprocal_normalised(y) );
        unsigned ignored;
        auto [q,q0] = product( r[1], y_ );
        ignored = sum_accumulate( q, r[1] );
        auto pp = product( q, y );
        ignored = minus_accumulate( r, pp );
        while ( r[1] > 0U || r[0] >= y ) {
            q += 1;
            ignored = minus_accumulate( r, y );
        }
        return q;
    }

    /// Singular quotient and remainder by "normalised" divisor with reciprocal.
    auto ratio_singular_normalised ( span<unsigned,2> x, unsigned y, unsigned y_ ) -> tuple< unsigned, unsigned >
    {
        array r { x[0], x[1] };
        auto q = ratio_singular_normalised_accumulate( r, y, y_ );
        return { q, r[0] };
    }

    /// Singular quotient and remainder by "normalised" divisor.
    auto ratio_singular_normalised ( span<unsigned,2> x, unsigned y ) -> tuple< unsigned, unsigned >
    {
        auto y_ = purple::reciprocal_normalised(y);
        return ratio_singular_normalised( x, y, y_ );
    }

    /// Accumulate remainder and return quotient by "normalised" divisor with reciprocal.
    auto ratio_normalised_accumulate ( span<unsigned,2> r, unsigned y, unsigned y_ ) -> array<unsigned,2>
    // requires 2^31 <= y < 2^32
    // requires y_ == reciprocal_normalised(y)
    {
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );
        assert( y_ == reciprocal_normalised(y) );
        array<unsigned,2> q {};
        while ( r[1] >= y ) {
            q[1] += 1;
            r[1] -= y;
        }
        q[0] = ratio_singular_normalised_accumulate(r,y,y_);
        return q;
    }

    /// Quotient and remainder by "normalised" divisor with reciprocal.
    auto ratio_normalised ( span<unsigned,2> x, unsigned y, unsigned y_ ) -> tuple< array<unsigned,2>, unsigned >
    {
        array r { x[0], x[1] };
        auto q = ratio_normalised_accumulate(r,y,y_);
        return { q, r[0] };
    }

    /// Quotient and remainder by "normalised" divisor.
    auto ratio_normalised ( span<unsigned,2> x, unsigned y ) -> tuple< array<unsigned,2>, unsigned >
    {
        auto y_ = purple::reciprocal_normalised(y);
        return ratio_normalised(x,y,y_);
    }
}
