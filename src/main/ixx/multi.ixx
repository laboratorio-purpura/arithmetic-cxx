// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <array>
#include <cassert>
#include <span>
#include <tuple>

/// Multi-word nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in a binary base B.
///
/// By multi-word integers we mean integers represented with variable quantity of words.
///
/// This module partition defines procedures with multi-word integer operands.
///
/// Requires, unless otherwise specified:
/// size(operand) ≥ 1

export module purple.arithmetic:multi;

import :bi;
import :word_concept;

using std::array;
using std::ignore;
using std::span;
using std::tie;

export namespace purple
{
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
    /// size(x) = size(y)

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
    /// size(x) = size(y)

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
    /// size(x) = size(y)

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
    /// size(x) = size(y)

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
    /// size(x) = size(y)

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
    /// size(x) = size(y)

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
    /// Let N = size(y).
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
    /// size(x) ≥ size(y)

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
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return carry bit.

    template <Word W>
    auto next_assign ( span<W> r, span<W const> x, W carry = W(0) ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        carry = next_assign( r[0], x[0], carry );
        for (auto i = 1uz; i != xz; ++i)
            carry = sum_assign( r[i], x[i], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return carry bit.

    template <Word W>
    auto sum_assign ( span<W> r, span<W const> x, W y, W carry = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        carry = sum_assign( r[0], x[0], y, carry );
        for (auto i = 1uz; i != xz; ++i)
            carry = sum_assign( r[i], x[i], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ size(x) ≥ size(y)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return carry bit.

    template <Word W>
    auto sum_assign ( span<W> r, span<W const> x, span<W const> y, W carry = W(0) ) noexcept -> W
    {
        auto const yz = size(y);
        assert( yz >= 1 );
        auto const xz = size(x);
        assert( xz >= yz );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != yz; ++i)
            carry = sum_assign( r[i], x[i], y[i], carry );
        for (auto i = yz; i != xz; ++i)
            carry = sum_assign( r[i], x[i], carry );
        return carry;
    }

    /// Product with excess.
    ///
    /// Requires:
    /// words(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return excess word.

    template <Word W>
    auto product_assign ( span<W> r, span<W const> x, W y, W excess = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != xz; ++i)
            excess = product_assign( r[i], x[i], y, excess );
        return excess;
    }

    /// Product.
    ///
    /// Requires:
    /// size(r) ≥ size(x) + size(y)
    ///
    /// Stores rd = size(x) + size(y) words into counted range [ begin(r), rd ).

    template <Word W>
    void product_accumulate ( span<W> r, span<W const> x, span<W const> y ) noexcept
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const yz = size(y);
        assert( yz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz + yz );
        assert( is_zero<W>( r.subspan(xz) ) );

        for (auto j = 0uz; j != yz; ++j)
        {
            auto excess = W{0u};
            for (auto i = 0uz; i != xz; ++i)
            {
                // x[i] × y[j]
                auto p = product( x[i], y[j], excess );
                // store into r[i+j]
                auto ri = i+j;
                auto c = sum_assign( r[ri], r[ri], p[0] );
                ignore = sum_assign( excess, p[1], c );
            }
            // store excess into r
            r[xz+j] = excess;
        }
    }

    /// Twice with excess.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return excess word.

    template <Word W>
    auto twice_assign ( span<W> r, span<W const> x, size_t z, W excess = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != xz; ++i)
            excess = twice_assign( r[i], x[i], z, excess );
        return excess;
    }

    /// Square.
    ///
    /// Requires:
    /// size(r) ≥ size(x) × 2
    ///
    /// Stores rd = size(x) + size(y) words into counted range [ begin(r), rd ).

    template <Word W>
    void square_accumulate ( span<W> r, span<W const> x ) noexcept
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz*2 );

        W storage [ 3 ];

        for (auto xi = 0uz; xi != xz; ++xi)
        {
            // xi ^ 2
            {
                // xi ^ 2 + carry
                auto p = product( x[xi], x[xi] );
                // store
                auto ri = xi+xi;
                auto rs = r.subspan(ri);
                ignore = sum_assign<W>( rs, rs, p );
            }
            // 2 * xi * xj
            for (auto xj = xi + 1uz; xj != xz; ++xj)
            {
                // xi * xj
                auto p = product( x[xi], x[xj] );
                // 2 * xi * xj
                auto t = span<W>( storage, 3 );
                t[2] = twice_assign<W>( t, p, 1uz );
                // store
                auto ri = xi+xj;
                auto rs = r.subspan(ri);
                ignore = sum_assign<W>( rs, rs, t );
            }
        }
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return borrow bit.

    template <Word W>
    auto previous_assign ( span<W> r, span<W const> x, W borrow = W(0) ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        borrow = previous_assign( r[0], x[0], borrow );
        for (auto i = 1uz; i != xz; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return borrow bit.

    template <Word W>
    auto difference_assign ( span<W> r, span<W const> x, W y, W borrow = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        borrow = difference_assign( r[0], x[0], y, borrow );
        for (auto i = 1uz; i != xz; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Requires:
    /// size(r) ≥ size(x) ≥ size(y)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return borrow bit.

    template <Word W>
    auto difference_assign ( span<W> r, span<W const> x, span<W const> y, W borrow = W(0) ) noexcept -> W
    {
        auto const yz = size(y);
        assert( yz >= 1 );
        auto const xz = size(x);
        assert( xz >= yz );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != yz; ++i)
            borrow = difference_assign( r[i], x[i], y[i], borrow );
        for (auto i = yz; i != xz; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        return borrow;
    }

    /// Half with remainder.
    ///
    /// Requires:
    /// size(q) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return remainder word.

    template <Word W>
    auto half_assign ( span<W> q, span<W const> x, size_t z ) noexcept -> W
    {
        auto const B = sizeof(W) * 8uz;
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(q);
        assert( rz >= xz );

        auto r = W(0);
        for (auto i = xz; i != 0; --i) {
            // halve current word
            auto r0 = W(0);
            tie( q[i-1], r0 ) = half( x[i-1], z );
            // add previous remainder
            auto [ r1, _ ] = twice( r, B - z );
            tie( q[i-1], ignore ) = sum( q[i-1], r1 );
            r = r0;
        }
        return r;
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

    template <Word W>
    auto division_assign ( span<W> q, span<W const> x, W y ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        assert( not_zero(y) );
        auto const qz = size(q);
        assert( qz >= xz );

        W storage [ xz + 1 ];

        // normalize operands, then divide with normalized algorithm.

        // 1. find normalization factor.
        auto factor = leading_zero_bits( y );

        // 2. normalize dividend and divisor.

        // 2.1. normalize divisor.
        auto [ ny, _ ] = twice( y, factor );

        // 2.2 normalize dividend.
        auto const nxd = xz + 1;
        auto nx = span( storage, nxd );
        nx[nxd-1] = twice_assign<W>( nx, x, factor );
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

    template <Word W>
    auto division_assign_step ( span<W> r, span<W const> x, span<W const> y ) -> W
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

        // compute tentative quotient q'
        auto q_ = array<W,2>{};
        auto r_ = W{};
        {
            // let x' = { x[yz-1}, x[yz] }
            auto x_ = array { x[yz-1], x[yz] };
            // let y' = y[yz-1]
            auto y_ = y[yz-1];
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
                    // q' × y[yz-2] > { x[yz-2], r' }
                    is_greater<W>( product( q_[0], y[yz-2] ), array { x[yz-2], r_ } )
                )
            ) {
                // q_ ← q_ - 1
                ignore = previous_assign<W>( q_, q_ );
                // r_ ← r_ + y[yz-1]
                carry = sum_assign( r_, r_, y[yz-1] );
                // carry = 1 ⇒ r_ ≥ B
            }
        }
        // invariant: q' - 1 ≤ q ≤ q' ≤ B

        // reduce q'
        // r ← x - q' × y
        auto borrow = W(0);
        {
            // let t = q' × y
            W t_ [ yz + 1 ];
            auto t = span<W>( t_, yz + 1 );
            t[yz] = product_assign<W>( t, y, q_[0] );
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

    template <Word W>
    void division_assign ( span<W> q, span<W> r, span<W const> x, span<W const> y )
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
        ignore = twice_assign<W>( ny, y, factor );
        // invariant: carry is zero

        // 2.2. normalize dividend.
        auto const nxd = xz + 1;
        auto nx = r.subspan( 0, nxd );
        nx[nxd-1] = twice_assign<W>( nx, x, factor );
        // invariant: { nx[-2], nx[-1] } < y

        // 3. divide with normalized operands.

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
