// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetics:division;

import :word_concept;

import :product;

namespace purple::arithmetics
{
    /// Normalized division with remainder.
    ///
    /// Computes by the "improved division by invariant integers" method.
    ///
    /// Requires:
    /// y is normalized
    /// x ÷ B < y
    /// iy = reciprocal_normalized(y)

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

    /// Normalized division with remainder.
    ///
    /// Computes by the "improved division by invariant integers" method.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

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

    /// Division with remainder.
    ///
    /// Computes by the classical or "school" method.
    ///
    /// Requirements:
    /// y is nonzero
    /// size(q) ≥ size(x)
    ///
    /// Stores qd ≤ size(x) words into counted range [ begin(q), qd ).
    ///
    /// @return remainder word.

    export
    template <Word W>
    auto division ( span<W> q, span<W const> x, W y ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        assert( not_zero(y) );
        auto const qz = size(q);

        W storage [ xz + 1 ];

        // 1. normalize divisor.

        // 1.1. normalization factor.
        auto factor = leading_zero_bits( y );

        // 1.2. normalize divisor.
        auto [ ny, _ ] = twice( y, factor );

        // 1.2. fix dividend, which becomes "strict".
        auto const nxd = xz + 1;
        auto nx = span( storage, nxd );
        nx[nxd-1] = twice<W>( nx, x, factor );
        // invariant: nx[xz] < y

        // 2. reciprocal approximation of normalized divisor.

        auto iy = reciprocal_normalized( ny );

        // 3. divide by reciprocal multiplication.

        auto r = nx[nxd-1];

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

    /// Division with remainder step.
    ///
    /// Computes one step of the classical or "school" method.
    ///
    /// Requirements:
    /// size(x) = size(y) + 1
    /// size(y) ≥ 2
    /// y is normalized
    /// x ÷ B < y
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd ≤ size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return quotient word.

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

    /// Division with remainder.
    ///
    /// Computes by the classical or "school" method.
    ///
    /// Requirements:
    /// size(x) ≥ size(y)
    /// size(y) ≥ 2
    /// y is nonzero
    /// size(q) ≥ size(x)
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores qd ≤ size(x) words into counted range [ begin(q), qd ).
    /// Stores rd ≤ size(x) + 1 words into counted range [ begin(r), rd ).

    export
    template <Word W>
    void division ( span<W> q, span<W> r, span<W const> x, span<W const> y )
    {
        auto const yz = size(y);
        assert( yz >= 2 );
        assert( not_zero( y[yz-1] ) ); // TODO
        assert( not_zero<W>( y ) );
        auto const xz = size(x);
        assert( xz >= yz );
        auto const qz = size(q);
        assert( qz >= xz );
        auto const rz = size(r);
        assert( rz >= xz + 1 );

        W storage [ yz ];

        // normalize the operands, then divide with the normalized algorithm.

        // let N = size(y)
        auto const N = yz;

        // 1. find normalization factor.
        auto factor = leading_zero_bits( y[N-1] );

        // 2. normalize dividend and divisor.

        // 2.1. normalize divisor.
        auto ny = span<W>( storage, N );
        ignore = twice<W>( ny, y, factor );
        // invariant: carry is zero

        // 2.2. normalize dividend.
        auto const nxd = xz + 1;
        auto nx = r.subspan( 0, nxd );
        nx[nxd-1] = twice<W>( nx, x, factor );
        // invariant: { nx[-2], nx[-1] } < y

        // 3. divide with normalized operands.

        auto iy = reciprocal_normalized( ny[yz-1] );

        // compute each quotient word one by one,
        // computing each by division of N+1 word dividend by N word divisor.
        // normalization of operands is key.

        // let M <- size(nx) - size(ny)
        auto M = nxd - yz;
        // invariant: M ≥ 1

        // for j from m to 0 excluding:
        for (auto j = M; j > 0; --j)
        {
            // let x' = { x[j+n], x[j+n-1}, ..., x[j] }
            auto x_ = nx.subspan( j-1, N+1 );

            q[j-1] = division_normal_strict<W>( x_, x_, ny, iy );
        }

        // 4. denormalize remainder.
        ignore = half<W>( nx, nx, factor );
    }
}
