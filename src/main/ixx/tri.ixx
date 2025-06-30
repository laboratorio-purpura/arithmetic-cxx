// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:tri;

import :mono;
import :duo;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::tuple;

// Tri-degree arithmetic.

export namespace purple
{
    /// Quotient and remainder by "normalised" operands.
    template <typename Integer, size_t BiDegree, size_t TriDegree>
    requires requires { BiDegree == 2uz; TriDegree == 3uz; }
    auto ratio_normalised ( span<Integer const,TriDegree> x, span<Integer const,BiDegree> y, Integer iy )
        -> tuple< Integer, array<Integer,BiDegree> >
    // requires B/2 <= y[1] < B
    // requires { x[1], x[2] } < y
    // require iy = ( ( B^3 - 1 ) / y ) - B
    {
        // 1. <q1,q0> <- v.u2
        auto q = product( iy, x[2] );
        // 2. <q1,q0> <- <q1,q0> + <u2,u1>
        auto x12 = array { x[1], x[2] };
        ignore = sum_accumulate( span(q), span(x12) );
        // 3. r1 <- (u1 - q1.d1) mod B
        auto q1y1 = product( q[1], y[1] );
        auto [ r1, _ ] = difference( x[1], q1y1[0] );
        // 4. <t1,t0> <- d0.q1
        auto t = product( y[0], q[1] );
        // 5. <r1,r0> <- (<r1,x0> - <t1,t0> - <d1,d0>) mod B^2
        auto r = array { x[0], r1 };
        ignore = difference_accumulate( span(r), span(t) );
        ignore = difference_accumulate( span(r), y );
        // 6. q1 <- (q1 + 1) mod B
        tie( q[1], ignore ) = sum( q[1], Integer(1) );
        // 7. if r1 >= q0
        if ( not_smaller( r[1], q[0] ) ) {
            // 8. q1 <- (q1 - 1) mod B
            tie( q[1], ignore ) = difference( q[1], Integer(1) );
            // 9. <r1,r0> <- (<r1,r0> + <d1,d0>) mod B^2
            ignore = sum_accumulate( span(r), y );
        }
        // 10. if <r1,r0> >= <d1,d0>
        if ( not_smaller( span(r), y ) ) [[unlikely]] {
            // 11. q1 <- q1 + 1
            tie( q[1], ignore ) = sum( q[1], 1 );
            // 12. <r1,r0> <- <r1,r0> - <d1,d0>
            ignore = difference_accumulate( span(r), y );
        }
        return { q[1], r };
    }
}
