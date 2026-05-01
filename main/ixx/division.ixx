// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:division;

import :assign;
import :compact;
import :difference;
import :half;
import :product;
import :reciprocal;
import :smaller;
import :sum;
import :twice;
import :word_concept;

namespace purple::arithmetics
{
    using std::array;
    using std::ignore;
    using std::size;
    using std::span;
    using std::tie;
    using std::tuple;

    /// Division of nonnegative 2-word integer `x` by 1-word normalized integer `y`.
    ///
    /// Returns the quotient and the remainder.
    ///
    /// Requires:
    /// y is normalized;
    /// iy = reciprocal_normalized(y);
    /// x ÷ β < y.
    ///
    /// This implementation applies the "improved division by invariant integers" method.

    export
    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto division_normal_strict ( span<W const,Bi> x, W y, W iy ) noexcept -> tuple< W, W >
    {
        assert( is_normalized( y ) );
        assert( is_smaller( x[1], y ) );

        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum<W>( t, t, x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _ ] = sum( t[1], W{1}, W{0} );
        // r = ( x - ( q × y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _ ] = difference( x[0], qy[0], W{0} );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            tie( q, ignore ) = previous( q, W{0} );
            tie( r, ignore ) = sum( r, y, W{0} );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            tie( q, ignore ) = next( q, W{0} );
            tie( r, ignore ) = difference( r, y, W{0} );
        }
        // terminate
        return { q, r };
    }

    /// Division of nonnegative integers `x` by `y`.
    ///
    /// Stores into `q` the `size(q)` least significant words of the quotient.
    /// Returns the remainder.
    ///
    /// Requires:
    /// y is nonzero.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1,
    /// augmented by the "improved division by invariant integers" method.

    export
    template <Word W>
    auto division ( span<W> q, span<W const> x, W y ) -> W
    {
        auto const xz = size(x);
        assert( not_zero(y) );
        auto const qz = size(q);

        // 1. normalize.

        // 1.1. normalization factor.
        auto factor = leading_zero_bits( y );

        // 1.2. normalize divisor.
        auto [ ny, _ ] = twice( y, factor );

        // 1.2. fix dividend, which becomes "strict".
        W nx_ [ xz + 1 ];
        auto nx = span<W>( nx_, xz + 1 );
        nx[xz] = twice<W>( nx, x, factor );
        // invariant: nx[-1] < y

        // 2. reciprocal approximation of normalized divisor.

        auto iy = reciprocal( ny );

        // 3. compute quotient, word by word.

        auto r = nx[xz];

        for (auto i = xz; i > min(qz,xz); --i) {
            tie( ignore, r ) = division_normal_strict<W,2uz>( array { nx[i-1], r }, ny, iy );
        }

        for (auto i = min(qz,xz); i > 0; --i) {
            tie( q[i-1], r ) = division_normal_strict<W,2uz>( array { nx[i-1], r }, ny, iy );
        }

        // 4. denormalize remainder.

        tie( r, ignore ) = half( r, factor );

        return r;
    }

    /// Division of nonnegative 3-word integer `x` by 2-word normalized integer `y`.
    ///
    /// Returns the quotient and the remainder.
    ///
    /// Requires:
    /// y is normalized
    /// iy = reciprocal_normalized(y)
    /// x ÷ β < y
    ///
    /// This implementation applies the "improved division by invariant integers" method.

    export
    template <Word W, size_t Tri, size_t Bi>
    requires  ( Tri == 3uz ) && ( Bi == 2uz )
    auto division_normal_strict ( span<W const,Tri> x, span<W const,Bi> y, W iy ) noexcept -> tuple< W, array<W,Bi> >
    {
        // 1. <q1,q0> ← v.u2
        auto q = product( iy, x[2] );
        // 2. <q1,q0> ← <q1,q0> + <u2,u1>
        auto x12 = array { x[1], x[2] };
        ignore = sum<W>( q, q, x12 );
        // 3. r1 ← (u1 - q1.d1) % B
        auto q1y1 = product( q[1], y[1] );
        auto [ r1, _ ] = difference( x[1], q1y1[0], W{0} );
        // 4. <t1,t0> ← d0.q1
        auto t = product( y[0], q[1] );
        // 5. <r1,r0> ← (<r1,x0> - <t1,t0> - <d1,d0>) % B^2
        auto r = array { x[0], r1 };
        ignore = difference<W>( r, r, t );
        ignore = difference<W>( r, r, y );
        // 6. q1 ← (q1 + 1) % B
        tie( q[1], ignore ) = sum( q[1], W{1}, W{0} );
        // 7. if r1 ≥ q0
        if ( not_smaller( r[1], q[0] ) ) {
            // 8. q1 ← (q1 - 1) % B
            tie( q[1], ignore ) = previous( q[1], W{0} );
            // 9. <r1,r0> ← (<r1,r0> + <d1,d0>) % B^2
            ignore = sum<W>( r, r, y );
        }
        // 10. if <r1,r0> ≥ <d1,d0>
        if ( not_smaller<W>( r, y ) ) [[unlikely]] {
            // 11. q1 ← q1 + 1
            tie( q[1], ignore ) = next( q[1], W{0} );
            // 12. <r1,r0> ← <r1,r0> - <d1,d0>
            ignore = difference<W>( r, r, y );
        }
        return { q[1], r };
    }

    /// Division of nonnegative (N+1)-word integer `x` by N-word normalized integer `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the remainder.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the remainder.
    /// Returns the quotient.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    /// size(x) = size(y) + 1
    /// size(y) ≥ 2
    /// y is normalized
    /// iy = reciprocal_normalized( top(y) )
    /// x ÷ β < y
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1,
    /// augmented by the "improved division by invariant integers" method.

    export
    template <Word W>
    auto division_normal_strict ( span<W> r, span<W const> x, span<W const> y, W iy ) -> W
    {
        auto const yz = size(y);
        assert( yz >= 2 );
        assert( is_normalized<W>( y ) );
        auto const xz = size(x);
        assert( xz == yz + 1 );
        assert( is_smaller<W>( x.subspan(1), y ) );
        auto const rz = size(r);
        assert( rz >= xz );

        // guess the quotient by division of the highest words.
        // restricted operands guarantee strict bound on the error.

        // compute tentative q' and r'
        array<W,2> q_ {};
        W r_ {};
        tie( q_[1], r_ ) = division_normal_strict<W,2>( array { x[yz], r_ }, y[yz-1], iy );
        tie( q_[0], r_ ) = division_normal_strict<W,2>( array { x[yz-1], r_ }, y[yz-1], iy );
        // invariant: q' - 2 ≤ q ≤ q' ≤ β
        assert( not_greater( q_[1], W{1} ) );

        // reduce q'
        while (
            // q' >= B
            not_zero( q_[1] ) ||
            // q' × y[yz-2] > { x[yz-2], r' }
            is_greater<W>( product( q_[0], y[yz-2] ), array { x[yz-2], r_ } )
        ) {
            // q_ ← q_ - 1
            ignore = difference<W>( q_, q_, W{1} );
            // r_ ← r_ + y[yz-1]
            auto carry = W(0);
            tie( r_, carry ) = sum( r_, y[yz-1], W{0} );
            // if r_ < β, repeat
            if ( not_zero( carry ) ) break;
        }
        // invariant: q' - 1 ≤ q ≤ q' < β
        assert( are_equal( q_[1], W{0} ) );

        // reduce q'
        // r ← x - q' × y
        auto borrow = W(0);
        {
            // let t = q' × y
            W t_ [ yz + 1 ];
            auto t = span<W>( t_, yz + 1 );
            t[yz] = product<W>( t, y, q_[0] );
            // r ← x - q' × y
            borrow = difference<W>( r, x, t );
        }
        // borrow = 1 ⇒ r < 0
        // r < 0 ⇒ q' > q
        if ( not_zero( borrow ) ) {
            // q' ← q' - 1
            tie( q_[0], ignore ) = previous( q_[0], W{0} );
            // r ← r + y
            ignore = sum<W>( r, r, y );
        }
        // invariant: q' = q

        // terminate
        return q_[0];
    }

    /// Division of nonnegative integers `x` by `y`.
    ///
    /// Stores into `q` the `size(q)` least significant words of the quotient.
    /// Stores into `r` the `size(r)` least significant words of the remainder.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the remainder.
    ///
    /// Requires:
    /// y is nonzero
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1,
    /// augmented by the "improved division by invariant integers" method.

    export
    template <Word W>
    void division ( span<W> q, span<W> r, span<W const> x, span<W const> y )
    {
        y = compact(y);

        auto const yz = size(y);
        assert( not_zero<W>( y ) );
        auto const xz = size(x);
        auto const qz = size(q);
        auto const rz = size(r);

        assert( yz > 0 );

        if ( yz == 1 ) {
            auto r_ = division( q, x, y[0] );
            assign<W>( r, r_ );
            return;
        }
        // invariant: size(y) > 1

        if ( xz < yz ) {
            assign<W>( q, W{0} );
            assign<W>( r, x );
            return;
        }
        // invariant: size(x) >= size(y)

        // 1. normalize.

        // 1.1. normalization factor.
        auto factor = leading_zero_bits( y[yz-1] );

        // 1.2. normalize divisor.
        W ny_ [ yz ];
        auto ny = span<W>( ny_, yz );
        ignore = twice<W>( ny, y, factor );

        // 1.2. fix dividend, which becomes "strict".
        W nx_ [ xz + 1 ];
        auto nx = span<W>( nx_, xz + 1 );
        nx[xz] = twice<W>( nx, x, factor );
        // invariant: size(nx) > size(ny)
        // invariant: nx ÷ β < ny

        // 2. compute reciprocal approximation of normalized divisor top word.

        auto iy = reciprocal( ny[yz-1] );

        // 3. compute quotient, word by word.

        auto M = xz + 1 - yz;
        // invariant: M ≥ 0

        for (auto i = M; i > min(qz,M); --i)
        {
            auto x_ = nx.subspan( i-1, yz+1 );
            ignore = division_normal_strict<W>( x_, x_, ny, iy );
        }

        for (auto i = min(qz,M); i > 0; --i)
        {
            auto x_ = nx.subspan( i-1, yz+1 );
            q[i-1] = division_normal_strict<W>( x_, x_, ny, iy );
        }

        // 4. denormalize remainder.

        ignore = half<W>( r, nx, factor );
    }
}
