// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
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

/// Bi-degree nonnegative integer arithmetics.
///
/// Let a *word* be a nonnegative integer in base B.
/// In a binary machine, B = 2 ^ bits.
///
/// By bi-degree integers we mean integers represented with two words.
///
/// This module partition defines procedures with bi-degree integer operands.
///
/// Requires, unless otherwise specified:
/// degree(operand) ≥ 1

export namespace purple
{
    /// Query procedures.

    /// Count of significant words.

    template <typename Integer, size_t Bi>
    auto degree ( span<Integer const,Bi> x ) noexcept -> size_t
    {
        auto i = x.size();
        while (i > 1 && is_zero( x[i-1] )) --i;
        return i;
    }

    /// Count of words.

    template <typename Integer, size_t Bi>
    auto words ( span<Integer const,Bi> x ) noexcept -> size_t
    {
        return x.size();
    }

    template <typename Integer, size_t Bi>
    auto words ( span<Integer,Bi> x ) noexcept -> size_t
    {
        return x.size();
    }

    /// Tests.

    /// Tests if zero.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_zero ( span<Word const,Bi> x ) noexcept -> bool
    {
        auto r = true;
        r = r && is_zero( x[0] );
        r = r && is_zero( x[1] );
        return r;
    }

    /// Tests if *not* zero.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_zero ( span<Word const,Bi> x ) noexcept -> bool
    {
        auto r = false;
        r = r || not_zero( x[0] );
        r = r || not_zero( x[1] );
        return r;
    }

    /// Tests if odd.
    ///
    /// In a binary machine, this means the least significant bit is 1.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_odd ( span<Word const,Bi> x ) noexcept -> bool
    {
        return is_odd( x[0] );
    }

    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_odd ( span<Word const,Bi> x ) noexcept -> bool
    {
        return not_odd( x[0] );
    }

    /// Tests if normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_normalized ( span<Word const,Bi> y ) noexcept -> bool
    {
        return is_normalized( y[1] );
    }

    /// Tests if *not* normalized.
    ///
    /// Normalized means B ÷ 2 ≤ y[1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 0.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_normalized ( span<Word const,Bi> y ) noexcept -> bool
    {
        return not_normalized( y[1] );
    }

    /// Tests if smaller.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_smaller ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 1;
    }

    /// Tests if *not* smaller.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_smaller ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( x[0], y[0], carry );
        tie( ignore, carry ) = difference( x[1], y[1], carry );
        return carry == 0;
    }

    /// Tests if greater.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_greater ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> Word
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 1;
    }

    /// Tests if *not* greater.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_greater ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        auto carry = Word(0);
        tie( ignore, carry ) = difference( y[0], x[0], carry );
        tie( ignore, carry ) = difference( y[1], x[1], carry );
        return carry == 0;
    }

    /// Tests if equal.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto is_equal ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
    {
        bool r = true;
        r = r && is_equal( x[0], y[0] );
        r = r && is_equal( x[1], y[1] );
        return r;
    }

    /// Tests if *not* equal.

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto not_equal ( span<Word const,Bi> x, span<Word const,Bi> y ) noexcept -> bool
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
    /// Requires:
    /// y is normalized

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto reciprocal_normalized ( span<Word const,Bi> y ) noexcept -> Word
    {
        auto v = reciprocal_normalized( y[1] );
        // p ← ( y[1] * v ) % B
        auto p = y[1] * v;
        // p ← ( p + y[0] ) % B
        p = p + y[0];
        if (p < y[0]) {
            v = v - 1;
            if (p >= y[1]) {
                v = v - 1;
                p = p - y[1];
            }
            // p ← ( p - y[1] ) % B
            p = p - y[1];
        }
        auto t = product( v, y[0] );
        // p ← ( p + t[1] ) % B
        p = p + t[1];
        if ( p < t[1] ) {
            v = v - 1;
            auto tp = array { t[0], p };
            if ( not_smaller( span<Word const,2>(tp), y ) ) {
                v = v - 1;
            }
        }
        return v;
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

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto next_assign ( span<Word> r, span<Word const,Bi> x, Word carry = Word(0) ) -> Word
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        carry = next_assign( r[0], x[0], carry );
        for (auto i = 1uz; i != xd; ++i)
            carry = sum_assign( r[i], x[i], Word(0), carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], Word(0), Word(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<Word> r, span<Word const,Bi> x, Word y, Word carry = Word(0) ) noexcept -> Word
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        carry = sum_assign( r[0], x[0], y, carry );
        for (auto i = 1uz; i != xd; ++i)
            carry = sum_assign( r[i], x[i], Word(0), carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], Word(0), Word(0), carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<Word> r, span<Word const,Bi> x, span<Word const,Bi> y, Word carry = Word(0) ) noexcept -> Word
    {
        using std::swap;

        auto const xd = degree(x);
        auto const yd = degree(y);
        auto const rz = words(r);

        if (xd < yd)
            return sum_assign(r,y,x,carry);

        for (auto i = 0uz; i != yd; ++i)
            carry = sum_assign( r[i], x[i], y[i], carry );
        for (auto i = yd; i != xd; ++i)
            carry = sum_assign( r[i], x[i], Word(0), carry );
        for (auto i = xd; i != rz; ++i)
            carry = sum_assign( r[i], Word(0), Word(0), carry );
        return carry;
    }

    /// Twice with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// z < bits
    /// words(r) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto twice_assign ( span<Word> r, span<Word const,Bi> x, size_t z, Word excess = Word(0) ) noexcept -> Word
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( rz >= xd );

        for (auto i = 0uz; i != xd; ++i)
            excess = twice_assign( r[i], x[i], z, excess );
        for (auto i = xd; i != rz; ++i)
            excess = sum_assign( r[i], Word(0), excess );
        return excess;
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring with a "borrow" or leaving a "remainder".

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<Word> r, span<Word const,Bi> x, Word y, Word borrow = Word(0) ) noexcept -> Word
    {
        auto const xd = degree(x);
        auto const rz = words(r);

        assert( rz >= xd );

        borrow = difference_assign( r[0], x[0], y, borrow );
        for (auto i = 1uz; i != xd; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        for (auto i = xd; i != rz; ++i)
            borrow = difference_assign( r[i], Word(0), borrow );
        return borrow;
    }

    /// Difference with borrow.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(r) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto difference_assign ( span<Word> r, span<Word const,Bi> x, span<Word const,Bi> y, Word borrow = Word(0) ) noexcept -> Word
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
            borrow = difference_assign( r[i], Word(0), borrow );
        return borrow;
    }

    /// Half N times with remainder.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// words(q) ≥ degree(x)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto half_assign ( span<Word> q, span<Word const,Bi> x, size_t z = 1 ) noexcept -> Word
    {
        auto const B = sizeof(Word) * 8uz;
        auto const xd = degree(x);
        auto const rz = words(q);

        auto r = x[0] & ((Word(1) << z) - Word(1));
        q[0] = x[0] >> z;
        if (xd > 1) {
            q[0] |= x[1] << (B - z);
            q[1] = x[1] >> z;
        }
        std::ranges::fill( q.subspan(xd), Word(0) );
        return r;
    }

    /// Normalized division with remainder.
    ///
    /// Computes by the "Improved division by invariant integers" method.
    ///
    /// Requires:
    /// x ÷ B < y
    /// y is normalized
    /// iy = reciprocal_normalized(y)

    template <typename Word, size_t Bi>
    requires ( Bi == 2uz )
    auto division_normalized ( span<Word const,Bi> x, Word y, Word iy ) noexcept -> tuple< Word, Word >
    {
        // t = ( r[1] × iy ) + x
        auto t = product( x[1], iy );
        ignore = sum_assign<Word,Bi>( t, t, x );
        // q = ( t[1] + 1 ) mod B
        auto [ q, _ ] = sum( t[1], Word(1) );
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