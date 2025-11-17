// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <array>
#include <cassert>
#include <span>
#include <tuple>

/// Bi-word nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in a binary base B.
///
/// By bi-word integers we mean integers represented with two words.
///
/// This module partition defines procedures with bi-word integer operands.

export module purple.arithmetic:bi;

import :word_concept;

using std::array;
using std::ignore;
using std::span;
using std::tie;
using std::tuple;

export namespace purple
{
    /// Test procedures.

    /// Tests if zero.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_zero ( span<W const,Bi> x ) noexcept -> bool
    {
        auto r = true;
        r = r && is_zero( x[0] );
        r = r && is_zero( x[1] );
        return r;
    }

    /// Tests if *not* zero.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_zero ( span<W const,Bi> x ) noexcept -> bool
    {
        auto r = false;
        r = r || not_zero( x[0] );
        r = r || not_zero( x[1] );
        return r;
    }

    /// Tests if odd.
    ///
    /// In a binary machine, this means the least significant bit is 1.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_odd ( span<W const,Bi> x ) noexcept -> bool
    {
        return is_odd( x[0] );
    }

    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_odd ( span<W const,Bi> x ) noexcept -> bool
    {
        return not_odd( x[0] );
    }

    /// Tests if normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_normalized ( span<W const,Bi> y ) noexcept -> bool
    {
        return is_normalized( y[1] );
    }

    /// Tests if *not* normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 0.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_normalized ( span<W const,Bi> y ) noexcept -> bool
    {
        return not_normalized( y[1] );
    }

    /// Comparison procedures.

    /// Tests if smaller.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_smaller ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        auto carry = W{0u};
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return not_zero( carry );
    }

    /// Tests if *not* smaller.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_smaller ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        auto carry = W{0u};
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return is_zero( carry );
    }

    /// Tests if greater.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_greater ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> W
    {
        auto carry = W{0u};
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return not_zero( carry );
    }

    /// Tests if *not* greater.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_greater ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        auto carry = W{0u};
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return is_zero( carry );
    }

    /// Tests if equal.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_equal ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        bool r = true;
        r = r && is_equal( x[0], y[0] );
        r = r && is_equal( x[1], y[1] );
        return r;
    }

    /// Tests if *not* equal.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_equal ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        bool r = false;
        r = r || not_equal( x[0], y[0] );
        r = r || not_equal( x[1], y[1] );
        return r;
    }

    /// Transform procedures.

    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Computes by the "improved division by invariant integers" method.
    ///
    /// Requires:
    /// y is normalized

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto reciprocal_normalized ( span<W const,Bi> y ) noexcept -> W
    {
        auto v = reciprocal_normalized( y[1] );
        // p ← ( y[1] * v ) % B
        auto [ p, _ ] = product ( y[1], v );
        // p ← ( p + y[0] ) % B
        tie( p, ignore ) = sum( p, y[0] );
        if ( is_smaller( p, y[0] ) ) {
            tie( v, ignore ) = previous( v );
            if ( not_smaller( p, y[1] ) ) {
                tie( v, ignore ) = previous( v );
                tie( p, ignore ) = difference( p, y[1] );
            }
            // p ← ( p - y[1] ) % B
            tie( p, ignore ) = difference( p, y[1] );
        }
        auto t = product( v, y[0] );
        // p ← ( p + t[1] ) % B
        tie( p, ignore ) = sum( p, t[1] );
        if ( is_smaller( p, t[1] ) ) {
            tie( v, ignore ) = previous( v );
            auto tp = array { t[0], p };
            if ( not_smaller<W,2>( tp, y ) ) {
                tie( v, ignore ) = previous( v );
            }
        }
        return v;
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return carry bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto next_assign ( span<W> r, span<W const,Bi> x, W carry = W{0u} ) -> W
    {
        assert( size(r) >= 2 );

        carry = next_assign( r[0], x[0], carry );
        carry = sum_assign( r[1], x[1], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<W> r, span<W const,Bi> x, W y, W carry = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        carry = sum_assign( r[0], x[0], y, carry );
        carry = sum_assign( r[1], x[1], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<W> r, span<W const,Bi> x, span<W const,Bi> y, W carry = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        carry = sum_assign( r[0], x[0], y[0], carry );
        carry = sum_assign( r[1], x[1], y[1], carry );
        return carry;
    }

    /// Twice with excess.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return excess word.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto twice_assign ( span<W> r, span<W const,Bi> x, size_t z, W excess = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        excess = twice_assign( r[0], x[0], z, excess );
        excess = twice_assign( r[1], x[1], z, excess );
        return excess;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring with a "borrow" or leaving a "remainder".

    /// Difference with borrow.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<W> r, span<W const,Bi> x, W y, W borrow = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        borrow = difference_assign( r[0], x[0], y, borrow );
        borrow = difference_assign( r[1], x[1], y, borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<W> r, span<W const,Bi> x, span<W const,Bi> y, W borrow = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        borrow = difference_assign( r[0], x[0], y[0], borrow );
        borrow = difference_assign( r[1], x[1], y[1], borrow );
        return borrow;
    }

    /// Half with remainder.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(q) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(q)`, 2 ).
    ///
    /// @return remainder word.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto half_assign ( span<W> q, span<W const,Bi> x, size_t z = 1 ) noexcept -> W
    {
        auto const B = sizeof(W) * 8uz;

        assert( size(q) >= 2 );

        auto r1 = W{0u};
        tie( q[1], r1 ) = half( x[1], z );
        tie( r1, ignore ) = twice( r1, B-z );

        auto r0 = W{0u};
        tie( q[0], r0 ) = half( x[0], z );
        tie( q[0], ignore ) = sum( q[0], r1 );

        return r0;
    }

    /// Normalized division with remainder.
    ///
    /// Computes by the "improved division by invariant integers" method.
    ///
    /// Requires:
    /// y is normalized
    /// x ÷ B < y
    /// iy = reciprocal_normalized(y)

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto division_normalized ( span<W const,Bi> x, W y, W iy ) noexcept -> tuple< W, W >
    {
        assert( is_normalized( y ) );
        assert( is_smaller( x[1], y ) );

        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_assign<W,Bi>( t, t, x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _ ] = sum( t[1], W(1) );
        // r = ( x - ( q × y ) ) mod B
        auto qy = product( q, y );
        auto [ r, _ ] = difference( x[0], qy[0] );
        // if r > t[0] : q = ( q - 1 ) mod B; r = ( r + y ) mod B
        if ( is_greater( r, t[0] ) ) {
            ignore = previous_assign( q, q );
            ignore = sum_assign( r, r, y );
        }
        // if r >= y : q = ( q + 1 ) mod B; r = ( r - y ) mod B
        if ( not_smaller( r, y ) ) [[unlikely]] {
            ignore = next_assign( q, q );
            ignore = difference_assign( r, r, y );
        }
        // terminate
        return { q, r };
    }
}