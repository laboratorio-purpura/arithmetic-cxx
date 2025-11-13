// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <array>
#include <cassert>
#include <span>
#include <tuple>

/// Multi-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in a binary base B.
///
/// By multi-degree integers we mean integers represented with variable quantity of words.
///
/// This module partition defines procedures with multi-degree integer operands.
///
/// Requires, unless otherwise specified:
/// degree(operand) ≥ 1

export module purple.arithmetic:multi;

import :bi;
import :word_concept;

using std::array;
using std::ignore;
using std::span;
using std::tie;

export namespace purple
{
    /// Query procedures.

    /// Count of significant words.

    template <Word W>
    auto degree ( span<W const> x ) noexcept -> size_t
    {
        auto i = x.size();
        while (i > 1 && is_zero( x[i-1] )) --i;
        return i;
    }

    /// Count of words.

    template <Word W>
    auto words ( span<W const> x ) noexcept -> size_t
    {
        return x.size();
    }

    template <Word W>
    auto words ( span<W> x ) noexcept -> size_t
    {
        return x.size();
    }

    /// Test procedures.

    /// Tests if zero.

    template <Word W>
    auto is_zero ( span<W const> x ) noexcept -> bool
    {
        bool r = true;
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r = r && is_zero(x[i]);
        return r;
    }

    /// Tests if *not* zero.

    template <Word W>
    auto not_zero ( span<W const> x ) noexcept -> bool
    {
        bool r = false;
        auto const xz = x.size();
        for (auto i = 0u; i != xz; ++i)
            r = r || not_zero(x[i]);
        return r;
    }

    /// Tests if smaller.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto is_smaller_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        assert( x.size() == y.size() );

        auto const z = y.size();
        auto b = W(0);
        for (auto i = 0uz; i != z; ++i)
            tie( ignore, b ) = difference( x[i], y[i], b );
        return not_zero( b );
    }

    /// Tests if smaller.

    template <Word W>
    auto is_smaller ( span<W const> x, span<W const> y ) -> bool;
    // deferred definition; see further down.

    /// Tests if *not* smaller.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto not_smaller_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller_isodegree( x, y );
    }

    /// Tests if *not* smaller.

    template <Word W>
    auto not_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( x, y );
    }

    /// Tests if greater.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto is_greater_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return is_smaller_isodegree( y, x );
    }

    /// Tests if greater.

    template <Word W>
    auto is_greater ( span<W const> x, span<W const> y ) -> bool;
    // deferred definition; see further down.

    /// Tests if *not* greater.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto not_greater_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller_isodegree( y, x );
    }

    /// Tests if *not* greater.

    template <Word W>
    auto not_greater ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( y, x );
    }

    /// Tests if equals.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto is_equal_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        auto const z = y.size();
        for (auto i = 0uz; i != z; ++i) {
            if ( not_equal( x[i], y[i] ) )
                return false;
        }
        return true;
    }

    /// Tests if equals.

    template <Word W>
    auto is_equal ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_equal(y,x);
        if (! is_equal_isodegree( x, y ))
            return false;
        for (auto i = yz; i != xz; ++i) {
            if ( not_zero( x[i] ) )
                return false;
        }
        return true;
    }

    /// Tests if *not* equals.
    ///
    /// Requires:
    /// degree(x) = degree(y)

    template <Word W>
    auto not_equal_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_equal_isodegree( x, y );
    }

    /// Tests if *not* equals.

    template <Word W>
    auto not_equal ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_equal( x, y );
    }

    /// Tests if normalized.
    ///
    /// Let N = degree(y).
    /// Normalized means B ÷ 2 ≤ y[N-1] < B.

    template <Word W>
    auto is_normalized ( span<W const> y ) noexcept -> bool
    {
        return is_normalized( y[ y.size() - 1 ] );
    }

    /// Transform procedures.

    /// Assigns a value.

    template <Word W>
    void assign ( span<W> r, W y ) noexcept
    {
        using std::ranges::fill;

        auto const rz = r.size();

        assert( rz >= 1 );

        r[0] = y;
        fill( r.subspan(1), W{0u} );
    }

    /// Assigns a value.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)

    template <Word W>
    void assign ( span<W> r, span<W const> y ) noexcept
    {
        using std::ranges::copy;
        using std::ranges::fill;

        auto const yz = y.size();
        auto const rz = r.size();

        assert( rz >= yz );

        copy( y, r.begin() );
        fill( r.subspan(yz), W{0u} );
    }

    /// Assigns zero.

    template <Word W>
    void clear ( span<W> r ) noexcept
    {
        using std::ranges::fill;

        fill( r, W{0u} );
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto next_assign ( span<W> r, span<W const> x, W carry = W(0) ) -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        carry = next_assign( r[0], x[0], carry );
        for (auto i = 1uz; i != xd; ++i)
            carry = sum_assign( r[i], x[i], carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], W(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto sum_assign ( span<W> r, span<W const> x, W y, W carry = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        carry = sum_assign( r[0], x[0], y, carry );
        for (auto i = 1uz; i != xd; ++i)
            carry = sum_assign( r[i], x[i], carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], W(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto sum_assign ( span<W> r, span<W const> x, span<W const> y, W carry = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const yd = degree(y);
        auto const rz = words(r);

        if (xd < yd)
            return sum_assign(r,y,x,carry);

        assert( xd >= yd );
        assert( yd >= 1 );
        assert( rz >= xd );

        for (auto i = 0uz; i != yd; ++i)
            carry = sum_assign( r[i], x[i], y[i], carry );
        for (auto i = yd; i != xd; ++i)
            carry = sum_assign( r[i], x[i], carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], W(0), carry );
        return carry;
    }

    /// Product with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto product_assign ( span<W> r, span<W const> x, W y, W excess = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        for (auto i = 0uz; i != xd; ++i)
            excess = product_assign( r[i], x[i], y, excess );
        for (auto i = xd; i != rz; ++i)
            excess = sum_assign( r[i], W(0), excess );
        return excess;
    }

    /// Product with excess.
    ///
    /// Requires:
    /// words(r) ≥ degree(x) + degree(y)

    template <Word W>
    auto product_accumulate ( span<W> r, span<W const> x, span<W const> y, W excess = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const yd = degree(y);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( yd >= 1 );
        assert( rz >= xd+yd );

        for (auto xi = 0uz; xi != xd; ++xi)
        {
            for (auto yi = 0uz; yi != yd; ++yi)
            {
                auto ri = xi+yi;
                auto [ p0, p1 ] = product( x[xi], y[yi], excess );
                auto c = sum_assign( r[ri], r[ri], p0 );
                tie( excess, ignore ) = sum( p1, c );
            }
            excess = sum_assign( r[xi+yd], r[xi+yd], excess );
        }
        for (auto i = xd+yd; i != rz; ++i) {
            excess = sum_assign( r[i], W(0), excess );
        }
        return excess;
    }

    /// Twice with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// z < bits
    /// words(r) ≥ degree(x)

    template <Word W>
    auto twice_assign ( span<W> r, span<W const> x, size_t z, W excess = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        for (auto i = 0uz; i != xd; ++i)
            excess = twice_assign( r[i], x[i], z, excess );
        for (auto i = xd; i != rz; ++i)
            excess = sum_assign( r[i], W(0), excess );
        return excess;
    }

    /// Square with excess.
    ///
    /// Requires:
    /// words(r) ≥ degree(x) × 2

    template <Word W>
    auto square_accumulate ( span<W> r, span<W const> x ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd*2 );

        W storage [ 3 ];

        W excess {};
        for (auto xi = 0uz; xi != xd; ++xi)
        {
            // xi ^ 2
            {
                // xi ^ 2 + carry
                auto p = product( x[xi], x[xi] );
                auto ri = xi+xi;
                auto rs = r.subspan(ri);
                excess = sum_assign<W>( rs, rs, p );
            }
            // 2 * xi * xj
            for (auto xj = xi + 1uz; xj != xd; ++xj)
            {
                // xi * xj
                auto p = product( x[xi], x[xj] );
                // 2 * xi * xj
                auto t = span<W>( storage, 3 );
                ignore = twice_assign<W>( t, p, 1uz );
                // store
                auto ri = xi+xj;
                auto rs = r.subspan(ri);
                ignore = sum_assign<W>( rs, rs, t );
            }
        }
        return excess;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto previous_assign ( span<W> r, span<W const> x, W borrow = W(0) ) -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        borrow = previous_assign( r[0], x[0], borrow );
        for (auto i = 1uz; i != xd; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        for (auto i = xd; i != rz; ++i)
            borrow = difference_assign( r[i], W(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <Word W>
    auto difference_assign ( span<W> r, span<W const> x, W y, W borrow = W(0) ) noexcept -> W
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( xd >= 1 );
        assert( rz >= xd );

        borrow = difference_assign( r[0], x[0], y, borrow );
        for (auto i = 1uz; i != xd; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        for (auto i = xd; i != rz; ++i)
            borrow = difference_assign( r[i], W(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// degree(x) ≥ degree(y)
    /// words(r) ≥ degree(x)

    template <Word W>
    auto difference_assign ( span<W> r, span<W const> x, span<W const> y, W borrow = W(0) ) noexcept -> W
    {
        auto const xd = x.size();
        auto const yd = y.size();
        auto const rz = words(r);

        if (xd < yd)
            return difference_assign( r, y, x, borrow );

        assert( xd >= yd );
        assert( yd >= 1 );
        assert( rz >= xd );

        for (auto i = 0uz; i != yd; ++i)
            borrow = difference_assign( r[i], x[i], y[i], borrow );
        for (auto i = yd; i != xd; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        for (auto i = xd; i != rz; ++i)
            borrow = difference_assign( r[i], W(0), borrow );
        return borrow;
    }

    /// Half with remainder.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(q) ≥ degree(x)

    template <Word W>
    auto half_assign ( span<W> q, span<W const> x, size_t z ) noexcept -> W
    {
        auto const B = sizeof(W) * 8uz;
        auto const xd = degree(x);
        auto const rz = words(q);

        assert( xd >= 1 );
        assert( rz >= xd );

        auto r = W(0);
        for (auto i = xd; i != 0; --i) {
            // halve current word
            auto r0 = W(0);
            tie( q[i-1], r0 ) = half( x[i-1], z );
            // add previous remainder
            auto [ r1, _ ] = twice( r, B - z );
            tie( q[i-1], ignore ) = sum( q[i-1], r1 );
            r = r0;
        }
        std::ranges::fill( q.subspan(xd), W(0) );
        return r;
    }

    /// Division with remainder.
    ///
    /// Computes by the classical or "school" method.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requirements:
    /// y is nonzero
    /// words(q) ≥ degree(x)

    template <Word W>
    auto division_assign ( span<W> q, span<W const> x, W y ) -> W
    {
        auto const xd = degree(x);
        auto const qz = words(q);

        assert( xd >= 1 );
        assert( not_zero(y) );
        assert( qz >= xd );

        W storage [ xd + 1 ];

        // normalize operands, then divide with normalized algorithm.

        // 1. find normalization factor.
        auto factor = leading_zero_bits( y );

        // 2. normalize dividend and divisor.

        // 2.1. normalize divisor.
        auto [ny,_] = twice( y, factor );

        // 2.2 normalize dividend.
        auto const nxd = xd + 1;
        auto nx = span( storage, nxd );
        ignore = twice_assign<W>( nx, x, factor );
        // invariant: { nx[-2], nx[-1] } < y

        // 3. divide with normalized.

        // compute each quotient word one by one,
        // computing each by division of 2 dividend words by singular divisor.
        // normalization of operands is key.

        auto iy = reciprocal_normalized( ny );

        auto [ _, r ] = division_normalized<W,2uz>( array { nx[nxd-1], W(0) }, ny, iy );
        for (auto i = nxd-1; i != 0; --i) {
            tie( q[i-1], r ) = division_normalized<W,2uz>( array { nx[i-1], r }, ny, iy );
        }

        // 4. denormalize remainder.
        ignore = half_assign( r, r, factor );

        return r;
    }

    /// Division with remainder step.
    ///
    /// Computes one step of the classical or "school" method.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requirements:
    /// words(x) = words(y) + 1
    /// words(y) ≥ 2
    /// y is normalized
    /// x ÷ B < y
    /// words(r) ≥ words(x)

    template <Word W>
    auto division_assign_step ( span<W> r, span<W const> x, span<W const> y ) -> W
    {
        auto const xz = words(x);
        auto const yz = words(y);
        auto const rz = words(r);

        assert( xz == yz + 1 );
        assert( yz >= 2 );
        assert( is_normalized<W>( y ) );
        assert( is_smaller<W>( x.subspan(1), y ) );
        assert( rz >= xz );

        // guess the quotient by division of the highest words.
        // restricted operands guarantee strict bound on the error.

        // let N = words(y)
        auto N = words(y);

        // compute tentative quotient q'
        auto q_ = array<W,2>{};
        auto r_ = W{};
        {
            // let x' = { x[N-1}, x[N] }
            auto x_ = array { x[N-1], x[N] };
            // let y' = y[N-1]
            auto y_ = y[N-1];
            // let q' = x' ÷ y'
            // let r' = x' % y'
            tie( q_, r_ ) = division( x_, y_ );
        }
        // invariant: q' - 2 ≤ q ≤ q'

        // reduce q'
        {
            auto carry = W(0);
            while (
                // r_ < B
                is_zero( carry ) && (
                    // q' >= B
                    not_zero( q_[1] ) ||
                    // q' × y[N-2] > { x[N-2], r' }
                    is_greater<W>( product( q_[0], y[N-2] ), array { x[N-2], r_ } )
                )
            ) {
                // q_ ← q_ - 1
                ignore = previous_assign<W>( q_, q_ );
                // r_ ← r_ + y[N-1]
                carry = sum_assign( r_, r_, y[N-1] );
                // carry = 1 ⇒ r_ ≥ B
            }
        }
        // invariant: q' - 1 ≤ q ≤ q' ≤ B

        // reduce q'
        // r ← x - q' × y
        auto borrow = W(0);
        {
            // let t = q' × y
            W t_ [ N + 1 ];
            auto t = span<W>( t_, N + 1 );
            ignore = product_assign<W>( t, y, q_[0] );
            // r ← x - t
            borrow = difference_assign<W>( r, x, t );
        }
        // borrow = 1 ⇒ r < 0
        // r < 0 ⇒ q' > q
        if ( not_zero( borrow ) ) {
            // q ← q - 1
            ignore = previous_assign( q_[0], q_[0] );
            // r ← r + y
            ignore = sum_assign<W>( r, r, y );
        }
        // invariant: q' = q

        // terminate
        return q_[0];
    }

    /// Division with remainder.
    ///
    /// Computes by the classical or "school" method.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requirements:
    /// degree(x) ≥ degree(y)
    /// degree(y) ≥ 2
    /// y is nonzero
    /// words(q) ≥ degree(x)
    /// words(r) ≥ degree(x)

    template <Word W>
    void division_assign ( span<W> q, span<W> r, span<W const> x, span<W const> y )
    {
        auto const xd = degree(x);
        auto const yd = degree(y);
        auto const qz = words(q);
        auto const rz = words(r);

        assert( xd >= yd );
        assert( yd >= 2 );
        assert( not_zero<W>( y ) );
        assert( qz >= xd );
        assert( rz >= xd + 1 );

        W storage [ yd ];

        // normalize the operands, then divide with the normalized algorithm.

        // let N = degree(y)
        auto const N = yd;

        // 1. find normalization factor.
        auto factor = leading_zero_bits( y[N-1] );

        // 2. normalize dividend and divisor.

        // 2.1. normalize divisor.
        auto ny = span<W>( storage, N );
        ignore = twice_assign<W>( ny, y, factor );

        // 2.2. normalize dividend.
        auto const nxd = xd + 1;
        auto nx = r.subspan( 0, nxd );
        ignore = twice_assign<W>( nx, x, factor );
        // invariant: { nx[-2], nx[-1] } < y

        // 3. divide with normalized operands.

        // compute each quotient word one by one,
        // computing each by division of N+1 degree dividend by N degree divisor.
        // normalization of operands is key.

        // let M <- degree(nx) - degree(ny)
        auto M = nxd - yd;
        // invariant: M ≥ 1

        // for j from m to 0 excluding:
        for (auto j = M; j > 0; --j)
        {
            // let x' = { x[j+n], x[j+n-1}, ..., x[j] }
            auto x_ = nx.subspan( j-1, N+1 );

            q[j-1] = division_assign_step<W>( x_, x_, ny );
        }

        // 4. denormalize remainder.
        ignore = half_assign<W>( nx, nx, factor );
    }
}

/// Deferred definitions.

namespace purple
{
    template <Word W>
    auto is_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_greater( y, x );
        auto c = is_smaller_isodegree( x.subspan(0,y.size()), y );
        for (auto i = yz; i != xz; ++i)
            c = c && is_zero( x[i] );
        return c;
    }

    template <Word W>
    auto is_greater ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_smaller( y, x );
        auto c = is_greater_isodegree( x.subspan(0,y.size()), y );
        for (auto i = yz; i != xz; ++i)
            c = c || not_zero( x[i] );
        return c;
    }
}
