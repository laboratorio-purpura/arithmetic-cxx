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
using std::tuple;

// Duo-degree arithmetic.

export namespace purple
{
    /// Properties.

    /// 1 if and only if x is smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto is_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry;
    }

    /// 1 if and only if x is *not* smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto not_smaller ( span<Integer const,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        return 1U - is_smaller( x, y );
    }

    /// Operators.

    // Accumulate sum, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto sum_accumulate ( span<Integer,Degree> x, Integer y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], y, carry );
        carry = sum_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    // Accumulate sum, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto sum_accumulate ( span<Integer,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        carry = sum_accumulate( x[0], y[0], carry );
        carry = sum_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate difference, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto difference_accumulate ( span<Integer,Degree> x, Integer y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        carry = difference_accumulate( x[0], y, carry );
        carry = difference_accumulate( x[1], Integer(0), carry );
        return carry;
    }

    // Accumulate difference, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto difference_accumulate ( span<Integer,Degree> x, span<Integer const,Degree> y ) noexcept -> Integer
    {
        auto carry = Integer(0);
        carry = difference_accumulate( x[0], y[0], carry );
        carry = difference_accumulate( x[1], y[1], carry );
        return carry;
    }

    // Accumulate twice N times, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto twice_accumulate ( span<Integer,Degree> x, size_t N ) noexcept -> Integer
    {
        auto carry = Integer(0);
        carry = twice_sum_accumulate( x[0], N, carry );
        carry = twice_sum_accumulate( x[1], N, carry );
        return carry;
    }

    // Accumulate half N times, rounded down.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto half_accumulate ( span<Integer,Degree> r, size_t N ) noexcept -> Integer
    {
        constexpr auto B = sizeof(Integer) * 8uz;
        r[0] >>= N;
        r[0] |= r[1] << (B - N);
        r[1] >>= N;
    }

    /// Quotient and remainder with "normalised" operands.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto ratio_normalised ( span<Integer const,Degree> x, Integer y, Integer iy ) noexcept -> tuple< Integer, Integer >
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
        ignore = sum_accumulate( span(t), x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _0 ] = sum( t[1], Integer(1) );
        // r = ( x - ( q * y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _1 ] = difference( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            ignore = difference_accumulate( q, 1 );
            ignore = sum_accumulate( r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            ignore = sum_accumulate( q, 1 );
            ignore = difference_accumulate( r, y );
        }
        // terminate
        return { q, r };
    }

    /// Quotient and remainder.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto ratio ( span<Integer const,Degree> x, Integer y ) noexcept -> tuple< array<Integer,2>, Integer >
    // requires y != 0
    {
        assert( 0 < y );

        auto q = array { Integer(0), Integer(0) };
        auto r = array { x[0], x[1] };

        // "normalise" dividend
        if ( r[1] >= y )
            tie( q[1], r[1] ) = ratio( r[1], y ); // TODO: ...with inverse
        assert( r[1] < y );

        // "normalise" divisor and remainder
        auto ylz = top_zeros( y );
        ignore = twice_accumulate( y, ylz );
        ignore = twice_accumulate( span(r), ylz );
        assert( 0x80000000U <= y );
        assert( y <= 0xFFFFFFFFU );

        // compute dividend inverse
        auto iy = inverse_normalised( y );

        // compute "normalised" ratio
        tie( q[0], r[0] ) = ratio_normalised( span<Integer const,2>(r), y, iy );

        // "denormalise" remainder
        half_accumulate( r[0], ylz );

        // terminate
        return { q, r[0] };
    }

    // Approximate inverse of "normalised" integer.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto inverse_normalised ( span<Integer const,Degree> y ) noexcept -> Integer
    // requires B/2 <= y[1] < B
    {
        auto v = inverse_normalised( y[1] );
        // p = ( y[1] * v ) mod B
        auto p = y[1] * v;
        // p = ( p + y[0] ) mod B
        p = p + y[0];
        if (p < y[0]) {
            v = v - 1;
            if (p >= y[1]) {
                v = v - 1;
                p = p - y[1];
            }
            // p = ( p - y[1] ) mod B
            p = p - y[1];
        }
        auto t = product( v, y[0] );
        // p = ( p + t[1] ) mod B
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

// Fix non-const parameter conversion.

export namespace purple
{
    /// Properties.

    /// 1 if and only if x is *not* smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto not_smaller ( span<Integer const,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }

    /// 1 if and only if x is *not* smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto not_smaller ( span<Integer,Degree> x, span<Integer const,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( span<Integer const,Degree>(x), y );
    }

    /// 1 if and only if x is *not* smaller than y, else 0.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto not_smaller ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return not_smaller<Integer,Degree>( span<Integer const,Degree>(x), span<Integer const,Degree>(y) );
    }

    // Operators.

    // Accumulate sum, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto sum_accumulate ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return sum_accumulate<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }

    // Accumulate difference, return carry.
    template <typename Integer, size_t Degree>
    requires requires { Degree == 2uz; }
    auto difference_accumulate ( span<Integer,Degree> x, span<Integer,Degree> y ) noexcept
    {
        return difference_accumulate<Integer,Degree>( x, span<Integer const,Degree>(y) );
    }
}