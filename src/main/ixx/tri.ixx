// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic:tri;

import :mono;
import :duo;
import :duo.span;

using std::array;
using std::ignore;
using std::size_t;
using std::span;
using std::tie;
using std::tuple;

/// Tri-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By tri-degree integers we mean integers represented with three words.
///
/// This module partition defines procedures with tri-degree integer operands.

export namespace purple
{
    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring with a "borrow" or leaving a "remainder".

    /// Normalized division with remainder.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Integer, size_t TriDegree, size_t BiDegree>
    requires  ( TriDegree == 3uz ) && ( BiDegree == 2uz )
    auto division_normalized ( span<Integer const,TriDegree> x, span<Integer const,BiDegree> y, Integer iy ) noexcept
        -> tuple< Integer, array<Integer,BiDegree> >
    {
        // 1. <q1,q0> ← v.u2
        auto q = product( iy, x[2] );
        // 2. <q1,q0> ← <q1,q0> + <u2,u1>
        auto x12 = array { x[1], x[2] };
        ignore = sum_assign( span(q), span(x12) );
        // 3. r1 ← (u1 - q1.d1) % B
        auto q1y1 = product( q[1], y[1] );
        auto [ r1, _ ] = difference( x[1], q1y1[0] );
        // 4. <t1,t0> ← d0.q1
        auto t = product( y[0], q[1] );
        // 5. <r1,r0> ← (<r1,x0> - <t1,t0> - <d1,d0>) % B^2
        auto r = array { x[0], r1 };
        ignore = difference_assign( span(r), span(t) );
        ignore = difference_assign( span(r), y );
        // 6. q1 ← (q1 + 1) % B
        ignore = sum_assign( q[1], Integer(1) );
        // 7. if r1 ≥ q0
        if ( not_smaller( r[1], q[0] ) ) {
            // 8. q1 ← (q1 - 1) % B
            ignore = previous_assign( q[1] );
            // 9. <r1,r0> ← (<r1,r0> + <d1,d0>) % B^2
            ignore = sum_assign( span(r), y );
        }
        // 10. if <r1,r0> ≥ <d1,d0>
        if ( not_smaller( span(r), y ) ) [[unlikely]] {
            // 11. q1 ← q1 + 1
            ignore = next_assign( q[1] );
            // 12. <r1,r0> ← <r1,r0> - <d1,d0>
            ignore = difference_assign( span(r), y );
        }
        return { q[1], r };
    }
}
